
//
// Created by nicomazz97 on 21/11/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/FirecloudServerInitializer.hpp"
#include "../DataSources/EventProvider.hpp"
#include <cassert>

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

#define TEST_FIREBASE_ID  "1234"

json generateNewUserBadJson() {
    json j;
    j[USER_ID_FIREBASE] = TEST_FIREBASE_ID;
    return j;
}

json generateNewUserJson() {
    json j;
    j[USER_ID_FIREBASE] = TEST_FIREBASE_ID;
    j[USER_LAT] = 12.34;
    j[USER_LNG] = 45.42;
    j[USER_MIN_MAG] = 4;
    j[USER_MAX_DIST] = 1000;
    j[USER_DELAY_NOTIFICATION] = 1000;
    j[USER_RECEIVE_TEST] = 1;
    return j;
}

json generateUserId(long id) {
    json j = generateNewUserJson();
    j[USER_ID] = id;
    return j;
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

    //test add user
    {
        stringstream output;
        auto r = client.request("POST", "/addUser", generateNewUserJson().dump());
        output << r->content.rdbuf();
        string respose = output.str();
        // the server respose must have an user id
        assert(respose.find("id") != string::npos);
        // get the id
        json respJson = json::parse(respose);
        long id = respJson["id"].get<long>();
        //check if the user is present in the db
        assert(up.getUser(id).id == id);
        //remove the test user
        User u = up.getUser(id);
        up.removeUser(u);
        assert(up.getUser(id).id != id);
    }
    // test bad json
    {
        stringstream output;

        auto r = client.request("POST", "/addUser", generateNewUserBadJson().dump());
        assert(r->status_code.find("400") != string::npos);
    }

    //test update user
    {
        User u;
        long id = up.persistUser(u);
        assert(u.id > 0);
        assert(id > 0);
        assert(up.getUser(id).firebaseID != TEST_FIREBASE_ID);
        string update_json = generateUserId(id).dump();

        stringstream output;
        auto r = client.request("POST", "/addUser", update_json);
        output << r->content.rdbuf();
        string respose = output.str();
        // the server respose must have an user id
        assert(respose.find("id") != string::npos);
        // get the id
        json respJson = json::parse(respose);
        long respose_id = respJson["id"].get<long>();
        assert (respose_id == id);
        //check if the user is present in the db
        assert(up.getUser(id).id == id);
        assert(up.getUser(id).firebaseID == TEST_FIREBASE_ID);
        //remove the test user
        up.removeUser(u);
        assert(up.getUser(id).id != id);
    }
    //test get user details
    {
        User u;
        long id = up.persistUser(u);
        assert(u.id > 0);
        assert(id > 0);
        stringstream ss;
        ss << "/user/" << id;

        stringstream output;
        auto r = client.request("GET", ss.str());
        output << r->content.rdbuf();
        string respose = output.str();
        assert(respose == UserBuilder::userToJson(u).dump(3));

        up.removeUser(u);
    }


    //todo
    server.stop();
    server_thread.join();

    cerr << "All is working for user_server!";
    return 0;
}
