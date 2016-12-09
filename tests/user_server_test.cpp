
//
// Created by nicomazz97 on 21/11/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/ServerInitializer.hpp"
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
    j[USER_RECEIVE_TEST] = true;
    return j;
}

json generateUserJson(long id, std::string secret) {
    json j = generateNewUserJson();
    j[USER_ID] = id;
    j[USER_SECRET_KEY] = secret;
    return j;
}

string getRequestRespose(std::string method, std::string path, std::string body = "") {
    static HttpClient client("localhost:8080");
    auto r = client.request(method, path, body);
    stringstream output;
    output << r->content.rdbuf();
    return output.str();
}

void testAddUser() {
    string respose = getRequestRespose("POST", "/addUser", generateNewUserJson().dump());

    // the server respose must have an user id
    assert(respose.find(USER_ID) != string::npos);
    assert(respose.find(USER_SECRET_KEY) != string::npos);

    // get the id
    json respJson = json::parse(respose);
    long id = respJson[USER_ID].get<long>();
    //check if the user is present in the db
    assert(UserPreferenceProvider::isUserPresent(id));
    //remove the test user
    User u = UserPreferenceProvider::getUser(id);
    UserPreferenceProvider::removeUser(u);
    assert(!UserPreferenceProvider::isUserPresent(id));
}

void testBadAddUser() {
    HttpClient client("localhost:8080");

    stringstream output;
    auto r = client.request("POST", "/addUser", generateNewUserBadJson().dump());
    output << r->content.rdbuf();
    cerr<< output.str();
    assert(r->status_code.find("400") != string::npos);

}

void testUpdateUser() {
    UserPreferenceProvider up;

    User u;
    u.secretKey = UserBuilder::generateRandomString();
    long id = up.persistUser(u);
    assert(u.id > 0);
    assert(id > 0);
    assert(u.secretKey.size() != 0);
    string update_json = generateUserJson(id, u.secretKey).dump();

    string respose = getRequestRespose("POST", "/addUser", update_json);

    // the server respose must have an user id and not a secret key
    assert(respose.find(USER_ID) != string::npos);
    assert(respose.find(USER_SECRET_KEY) == string::npos);
    // get the id
    json respJson = json::parse(respose);
    long respose_id = respJson[USER_ID].get<long>();
    assert (respose_id == id);
    //check if the user is present in the db
    assert(up.getUser(id).id == id);
    assert(up.getUser(id).secretKey == u.secretKey);
    //remove the test user
    up.removeUser(u);
    assert(up.getUser(id).id != id);
}

void testGetUserDetails() {
    UserPreferenceProvider up;

    User u;
    u.secretKey = UserBuilder::generateRandomString();
    long id = up.persistUser(u);
    assert(u.id > 0);
    assert(id > 0);
    stringstream ss;
    ss << "/user/" << id;

    string respose = getRequestRespose("GET", ss.str());
    assert(respose == UserBuilder::userToJson(u).dump(3));

    up.removeUser(u);

}

int main() {
    HttpServer server(8080, 1);

    FCMServer::initServer(server);

    thread server_thread([&server]() {
        server.start();
    });
    this_thread::sleep_for(chrono::seconds(1));

    testAddUser();
    testBadAddUser();
    testUpdateUser();
    testGetUserDetails();

    server.stop();
    server_thread.join();

    cerr << "All is working for user_server!";
    return 0;
}
