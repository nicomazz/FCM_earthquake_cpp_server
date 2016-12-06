//
// Created by nicomazz97 on 20/11/16.
//

#include "UserBuilder.hpp"


User UserBuilder::buildFromJson(std::string json_string) {
    syslog(LOG_INFO, "building new user");

    try {
        json json_content = json::parse(json_string);
        User u;
        try {
            u.id = json_content[USER_ID].get<long>();
            u.secretKey = json_content[USER_SECRET_KEY].get<std::string>();
        } catch (...) {
            //user id not found, generate it and a secret key
            u.id = NEW_USER_DEFAULT_ID;
            u.secretKey = generateRandomString(SECRET_KEY_LENGTH);
        }
        u.firebaseID = json_content[USER_ID_FIREBASE].get<std::string>();
        u.lat = json_content[USER_LAT].get<double>();
        u.lng = json_content[USER_LNG].get<double>();
        u.minMagPreference = json_content[USER_MIN_MAG].get<double>();
        u.maxDistancePreference = json_content[USER_MAX_DIST].get<double>();
        u.minMillisNotificationDelay = json_content[USER_DELAY_NOTIFICATION].get<long>();
        u.lastNotificationMillis = 0;
        u.receiveRealTimeNotification = json_content[USER_RECEIVE_TEST].get<bool>();
        return u;
    } catch (std::exception e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("json string with bad format, cannot parse the user");
    }
}

json UserBuilder::userToJson(User &u) {
    json json_content;
    json_content[USER_ID] = u.id;
    json_content[USER_LAT] = u.lat;
    json_content[USER_LNG] = u.lng;
    json_content[USER_LAST_ACTIVITY] = u.lastActivity;
    return json_content;
}

std::string UserBuilder::generateRandomString(int l) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(97, 126); // printable ascii chars
    std::string result;
    result.resize(l);
    for (auto &c : result)
        c = (char) dis(gen);
    return result;
}


