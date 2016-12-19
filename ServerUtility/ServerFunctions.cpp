//
// Created by nicomazz97 on 19/12/16.
//

#include <DataSources/UserPreferenceProvider.hpp>
#include <Firebase/FirebaseNotificationManager.hpp>
#include <Models/User/UserBuilder.hpp>
#include <DataSources/EventProvider.hpp>
#include <Models/Event/EventBuilder.hpp>
#include "ServerFunctions.hpp"

std::string FCMServer::generateAllUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestUsersFromDB();
    json jsonObj;

    for (User &u : allUsers)
        jsonObj.push_back(UserBuilder::userToJson(u));
    return jsonObj.dump(3);
}

std::string FCMServer::generateActiveUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestActiveUsers();

    json jsonObj = json::array();

    for (User &u : allUsers)
        if (u.hasPosition())
            jsonObj.push_back(UserBuilder::userToJson(u));

    return jsonObj.dump(3);
}

std::string FCMServer::generateRecentUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestRecentUsers();

    json jsonObj = json::array();

    for (User &u : allUsers) {
        if (u.hasPosition())
            jsonObj.push_back(UserBuilder::userToJson(u));
    }

    return jsonObj.dump(3);
}

std::string FCMServer::generateDetectedEvents() {
    std::vector<Event> realTimeEvents = EventProvider::requestDetectorEvents();
    json jsonObj = json::array();

    for (Event &e : realTimeEvents)
        jsonObj.push_back(EventBuilder::eventToJson(e));

    return jsonObj.dump(3);
}

void FCMServer::outputHttpOKStringResponse(std::string s, Response response, std::string headers) {
    stringstream content_stream;
    content_stream << s;
    content_stream.seekp(0, ios::end);

    *response << "HTTP/1.1 200 OK\r\n"
              << "Content-Length: " << content_stream.tellp() << "\r\n"
              << "Access-Control-Allow-Origin: *";
    if (headers.size() != 0)
        *response << "\r\n" << headers;
    *response << "\r\n\r\n" << content_stream.rdbuf();
}

void FCMServer::outputHttpBADStringResponse(std::string s, Response response) {
    *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << s.length() << "\r\n\r\n"
              << s;
}
