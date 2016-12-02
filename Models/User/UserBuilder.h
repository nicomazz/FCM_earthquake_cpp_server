//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERBUILDER_H
#define SIMPLE_WEB_SERVER_USERBUILDER_H


#include <stdexcept>
#include <Utility/json.hpp>
#include <syslog.h>
#include "User.hpp"

using json = nlohmann::json;
using namespace std;

class UserBuilder {


public:
    /**
     * crea un utente da una stringa json. Nel caso ci siano problemi di parse
     * o di attributi mancanti viene lanciata un'eccezione
     * @param json_string
     * @return
     */
    static User buildFromJson(std::string json_string) {
        syslog(LOG_INFO, "building new user");

        try {
            json json_content = json::parse(json_string);
            User u;
            try { u.id = json_content["id"].get<long>(); } catch (...) { u.id = -1; } // può non esserci
            u.firebaseID = json_content["idFirebase"].get<std::string>();
            u.lat = json_content["lat"].get<double>();
            u.lng = json_content["lng"].get<double>();
            u.minMagPreference = json_content["minMagnitude"].get<double>();
            u.maxDistancePreference = json_content["maxDistance"].get<double>();
            u.minMillisNotificationDelay = json_content["minTemporalDistanceBwNotification"].get<double>();
            u.lastNotificationMillis = 0;
            return u;
        } catch (std::exception e) {
            syslog(LOG_INFO, e.what());
            throw std::invalid_argument("json string with bad format, cannot create the user");
        }
    }

    static json userToJson(User & u){
        json json_content;
        json_content["id"] = u.id;
        json_content["lat"] = u.lat;
        json_content["lng"] = u.lng;
        return json_content;
    }
};


#endif //SIMPLE_WEB_SERVER_USERBUILDER_H
