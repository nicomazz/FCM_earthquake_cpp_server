//
// Created by nicomazz97 on 02/12/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/ServerInitializer.hpp"
#include <cassert>
#include <Detector/ReportParserHTTP.hpp>
#include <DataSources/EventProvider.hpp>
#include "../Detector/SimpleEQDetector.hpp"

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

#define TEST_POWER 10

json generateGoodRequest(const User &user) {
    json j;
    j[REPORT_USER_ID] = user.id;
    j[REPORT_SECRET_KEY] = user.secretKey;
    j[REPORT_POWER] = TEST_POWER;
    return j;
}

json generateUserMismatchRequest() {
    json j;
    j[REPORT_USER_ID] = 215536524562265L;
    j[REPORT_SECRET_KEY] = "...";
    j[REPORT_POWER] = TEST_POWER;
    return j;
}

json generateUserMismatchSecretKeyRequest(const User &user) {
    json j;
    j[REPORT_USER_ID] = user.id;
    j[REPORT_SECRET_KEY] = "";
    j[REPORT_POWER] = TEST_POWER;
    return j;
}

json generateMissingParamsRequest() {
    json j;
    j[REPORT_POWER] = TEST_POWER;
    return j;
}

User generateNewUser() {
    User newUser;
    newUser.lat = 45 + (rand() / 100 * 0.00001f);
    newUser.lng = 12 + (rand() / 100 * 0.00001f);
    newUser.secretKey = "123";
    long insertedId = UserPreferenceProvider::persistUser(newUser);
    assert(insertedId >= 0);
    assert(newUser.id == insertedId);
    return newUser;
}

void reportParserTests() {
    UserPreferenceProvider up;

    //test missing params
    {
        string bad = generateMissingParamsRequest().dump();
        try {
            ReportParserHTTP::parseRequest(bad);
            assert(false);
        } catch (std::invalid_argument e) {
            assert(string(e.what()).find("missing") != string::npos);
        }
    }
    //test correct request parse
    {
        try {
            User newUser = generateNewUser();
            string good = generateGoodRequest(newUser).dump();
            Report r = ReportParserHTTP::parseRequest(good);
            assert(r.u.id == newUser.id);
            assert(r.power == TEST_POWER);
            up.removeUser(newUser);
        } catch (std::exception e) {
            assert((false && "problems in correct request parse"));
        }
    }
    //mismatch user id request
    {
        string mismatch = generateUserMismatchRequest().dump();
        try {
            Report r = ReportParserHTTP::parseRequest(mismatch);
            assert(false);
        } catch (std::invalid_argument e) {
            assert(string(e.what()).find("not in database") != string::npos);
        }
    }
    //mismatch secreyKey id
    {
        User newUser = generateNewUser();
        string mismatch = generateUserMismatchSecretKeyRequest(newUser).dump();
        try {
            Report r = ReportParserHTTP::parseRequest(mismatch);
            assert(false);
        } catch (std::invalid_argument e) {
            assert(string(e.what()).find("mismatch") != string::npos);
        }
        up.removeUser(newUser);
    }
}

void sendReportToServer(User &u) {
    HttpClient client("localhost:8080");

    auto r = client.request("POST", "/report", generateGoodRequest(u).dump());
    assert(r->status_code.find("400") == string::npos);
    stringstream output;
    output << r->content.rdbuf();
    std::string respose = output.str();
    assert(respose.find("Report send") != string::npos);
}

void sendReportTest() {
    UserPreferenceProvider up;
    User u = generateNewUser();
    sendReportToServer(u);
    up.removeUser(u);
}

void removeOldReportsTest() {
    SimpleEQDetector detector;
    UserPreferenceProvider up;

    User newUser = generateNewUser();
    string goodRequest = generateGoodRequest(newUser).dump();
    Report r1 = ReportParserHTTP::parseRequest(goodRequest);
    r1.millis -= REPORT_TTL * 2 + 10;
    Report r2 = ReportParserHTTP::parseRequest(goodRequest);
    r2.millis += 3;
    Report r3 = ReportParserHTTP::parseRequest(goodRequest);
    r3.millis -= REPORT_TTL * 2 + 60;
    Report r4 = ReportParserHTTP::parseRequest(goodRequest);
    r4.millis += 6;
    Report r5 = ReportParserHTTP::parseRequest(goodRequest);
    r5.millis -= REPORT_TTL * 2 + 100;

    vector<Report> rs{r1, r2, r3, r4, r5};
    detector.addReports(rs);
    assert(detector.size() == 2); // 1,3,5 must be deleted
    detector.clear();
    assert(detector.size() == 0);
    up.removeUser(newUser);
}

int main() {
    HttpServer server(8080, 1);
    FCMServer::initServer(server);
    thread server_thread([&server]() {
        server.start();
    });
    this_thread::sleep_for(chrono::seconds(1));


    HttpClient client("localhost:8080");
    UserPreferenceProvider up;

    // server test:
    reportParserTests();
    sendReportTest();

    // SimpleEQDetector test:
    SimpleEQDetector detector;

    //add request and clear
    {
        User newUser = generateNewUser();
        string good = generateGoodRequest(newUser).dump();

        Report r = ReportParserHTTP::parseRequest(good);
        detector.addReport(r);
        detector.addReport(r); // same as precedent, not size increace
        assert(detector.size() == 1);
        detector.clear();
        assert(detector.size() == 0);
        up.removeUser(newUser);
    }

    // remove old reports test
    removeOldReportsTest();

    // test send notification
    {
        const int n_users = MIN_USER_DETECTED_EQ;
        vector<User> utenti;
        for (int i = 0; i < n_users; i++)
            utenti.push_back(generateNewUser());

        int startEventNumber = EventProvider::requestEventFromDB().size();
        detector.clear();
        // generate n requests
        for (int i = 0; i < n_users; i++) {
            string goodRequest = generateGoodRequest(utenti[i]).dump();
            Report r = ReportParserHTTP::parseRequest(goodRequest);

            detector.addReport(r);
            r.millis += i;
        }

        // must have sent a notification in the previous 100 ms
        long timeDiff = TimeUtils::getCurrentMillis() - detector.millisLastNotifySend;
        assert(timeDiff < 100);
        assert(startEventNumber + 1 == (int) EventProvider::requestEventFromDB().size());

        Event notified = detector.getLastEventNotified();
        assert(notified.contributorId.size() != 0);
        //cerr << notified.contributorId << endl;
        for (int i = 0; i < n_users; i++) {
            User u = utenti[i];
            stringstream ss;
            ss << u.id;
            string user_id_s = ss.str();
            assert(notified.contributorId.find(user_id_s) != string::npos);
        }

        detector.clear();
        for (User &u: utenti)
            up.removeUser(u);
    }

    server.stop();
    server_thread.join();

    cerr << "All is working for simple detector test!";
    return 0;
}
