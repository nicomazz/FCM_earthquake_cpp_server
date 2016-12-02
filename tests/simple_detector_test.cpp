//
// Created by nicomazz97 on 02/12/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/FirecloudServerInitializer.hpp"
#include <cassert>
#include <Detector/ReportParserHTTP.hpp>
#include "../Detector/SimpleEQDetector.hpp"

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

#define TEST_FIREBASE_ID  "1234"

json generateRequest(const User &user) {
    json j;
    j[REPORT_USER_ID] = user.id;
    j[REPORT_FIREBASE_ID] = user.firebaseID;
    j[REPORT_POWER] = 10;
    return j;
}

json generateUserMismatchRequest() {
    json j;
    j[REPORT_USER_ID] = 215536524562265L;
    j[REPORT_FIREBASE_ID] = "...";
    j[REPORT_POWER] = 10;
    return j;
}

json generateUserMismatchFirebaseIdRequest(const User &user) {
    json j;
    j[REPORT_USER_ID] = user.id;
    j[REPORT_FIREBASE_ID] = "123";
    j[REPORT_POWER] = 10;
    return j;
}
json generateBadRequest() {
    json j;
    j[REPORT_POWER] = 10;
    return j;
}
User generateNewUser() {
    UserPreferenceProvider up;
    User newUser;
    newUser.lat = 45;
    newUser.lng = 12;
    long insertedId = up.persistUser(newUser);
    assert(insertedId >= 0);
    assert(newUser.id == insertedId);
    return newUser;
}

void reportParserTests() {
    UserPreferenceProvider up;

    //test bad request parse
    {
        string bad = generateBadRequest().dump();
        try {
            ReportParserHTTP::parseRequest(bad);
            assert(false);
        } catch (std::invalid_argument e){
            assert(string(e.what()).find("missing") != string::npos);
        }
    }
    //test correct request parse
    {
        User newUser = generateNewUser();
        string good = generateRequest(newUser).dump();
        Report r = ReportParserHTTP::parseRequest(good);
        up.removeUser(newUser);
    }
    //mismatch user id request
    {
        string mismatch = generateUserMismatchRequest().dump();
        try {
            Report r = ReportParserHTTP::parseRequest(mismatch);
            assert(false);
        } catch (std::invalid_argument e) {
            assert(string(e.what()).find("not found") != string::npos);
        }
    }
    //mismatch firebase id
    {
        User newUser = generateNewUser();
        string mismatch = generateUserMismatchFirebaseIdRequest(newUser).dump();
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

    auto r = client.request("POST", "/report", generateRequest(u).dump());
    assert(r->status_code.find("400") == string::npos);
}

void sendReportTest() {
    UserPreferenceProvider up;
    User u = generateNewUser();
    sendReportToServer(u);
    up.removeUser(u);
}

void removeOldReportsTest(){
    SimpleEQDetector detector;
    UserPreferenceProvider up;

    User newUser = generateNewUser();
    string goodRequest = generateRequest(newUser).dump();
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
        //Start server
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
        string good = generateRequest(newUser).dump();

        Report r = ReportParserHTTP::parseRequest(good);
        detector.addReport(r);
        detector.addReport(r); // same as precedent, not size increace
        assert(detector.size() == 1);
        r.millis++; //different millis, size increace
        detector.addReport(r);
        assert(detector.size() == 2);
        detector.clear();
        assert(detector.size() == 0);
        up.removeUser(newUser);
    }

    // remove old reports test
    removeOldReportsTest();

    // test send notification
    {
        User newUser = generateNewUser();
        string goodRequest = generateRequest(newUser).dump();
        Report r1 = ReportParserHTTP::parseRequest(goodRequest);
        detector.clear();
        for (int i = 0; i < MIN_NEAR_REPORTS+1 ; i++) { // put different same report to trigger a notification
            detector.addReport(r1);
            r1.millis++;
            if ( i != MIN_NEAR_REPORTS)
                assert(detector.size() == i+1);
        }
        // must have send a notify in the previous 100 ms
        long timeDiff = TimeUtils::getCurrentMillis() - detector.millisLastNotifySend;
        assert(timeDiff < 100);
    }


    //todo
    server.stop();
    server_thread.join();

    cerr << "All is working for user_server!";
    return 0;
}
