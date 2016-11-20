//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERBUILDER_H
#define SIMPLE_WEB_SERVER_USERBUILDER_H


#include <stdexcept>
#include <Utility/json.hpp>
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
    static User buildFromJson(std::string json_string){

        json json_content = json::parse(json_string);
        User u;
        try{ u.id = getLong(json_content,"id");} catch (...){ u.id = -1;} // può non esserci
        u.firebaseID = getString(json_content, "idFirebase");
        u.lat = getDouble(json_content, "lat");
        u.lng = getDouble(json_content, "lng");
        u.minMagPreference = getDouble(json_content, "minMagnitude");
        u.maxDistancePreference = getDouble(json_content, "maxDistance");
        u.minMillisNotificationDelay = getLong(json_content, "minDistanceNotification");
        u.lastNotificationMillis = 0;

        return u;
    }

private:
    //TODO usare template!
    static std::string getString(json &j, string key) {
        auto i = j.find(key);
        if (i != j.end()) {
            cout<<"i.value: "<<i.value()<<"\n";
            return i.value();
        }
        throw std::invalid_argument("Missing argument: "+key+"!");
    }

    static double getDouble(json &j, string key) {
        auto i = j.find(key);
        if (i != j.end()) {
            cout<<"i.value: "<<i.value()<<"\n";
            return i.value();
        }
        throw std::invalid_argument("Missing argument: "+key+"!");
    }
    static long getLong(json &j, string key) {
        auto i = j.find(key);
        if (i != j.end()) {
            cout<<"i.value: "<<i.value()<<"\n";
            return i.value();
        }
        throw std::invalid_argument("Missing argument: "+key+"!");
    }
};


#endif //SIMPLE_WEB_SERVER_USERBUILDER_H
