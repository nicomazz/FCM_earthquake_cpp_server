//
// Created by nicomazz97 on 01/12/16.
//

#include <syslog.h>
#include <Models/User/UserBuilder.h>
#include <DataSources/UserPreferenceProvider.hpp>
#include "ReportParserHTTP.hpp"


Report ReportParserHTTP::parseRequest(std::string body) {
    try {
        json json_content = json::parse(body);
        int id = json_content[REPORT_USER_ID].get<long>();
        UserPreferenceProvider up;
        User u = up.getUser(id);
        std::string user_true_firebase_id = json_content[REPORT_FIREBASE_ID].get<std::string>();

        if (u.id <= 0)
            throw std::invalid_argument("User not found!");
        if ( user_true_firebase_id != u.firebaseID)
            throw std::invalid_argument("User firebaseID mismatch!");

        //todo verificare che sia effettivamente un utente
        int power = json_content[REPORT_POWER].get<int>();
        return Report(u, power);
    } catch (std::invalid_argument e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("json string with bad format: " + string(e.what()) + ", cannot create the Report");
    } catch (std::domain_error e){
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("json string with bad format: missing fields, cannot create the Report");

    }

}

