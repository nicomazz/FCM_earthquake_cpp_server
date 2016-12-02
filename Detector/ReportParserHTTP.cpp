//
// Created by nicomazz97 on 01/12/16.
//

#include <syslog.h>
#include <Models/User/UserBuilder.h>
#include "ReportParserHTTP.hpp"



Report ReportParserHTTP::parseRequest(std::string body) {
    syslog(LOG_INFO, "building new user");

    try {
        json json_content = json::parse(body);
        User u;
        u.id = json_content["id"].get<long>();
        u.firebaseID = json_content["idFirebase"].get<std::string>();
        //todo verificare che sia effettivamente un utente
        int power = json_content["strong"].get<int>();
        return Report(u,power);
    } catch (std::exception e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("json string with bad format, cannot create the Report");
    }

}

