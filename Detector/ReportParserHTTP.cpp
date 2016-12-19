//
// Created by nicomazz97 on 01/12/16.
//

#include <syslog.h>
#include <Models/User/UserBuilder.hpp>
#include <DataSources/UserPreferenceProvider.hpp>
#include "ReportParserHTTP.hpp"


Report ReportParserHTTP::parseRequest(std::string body) {
    try {
        json json_content = json::parse(body);
        User requester = getUserFromReportJson(json_content);
        UserPreferenceProvider::checkValidUserInDB(requester);

        User userInDb = UserPreferenceProvider::getUser(requester.id);
        userInDb.lastActivity = TimeUtility::getCurrentMillis();
        UserPreferenceProvider::updateUser(userInDb);

        int power = json_content[REPORT_POWER].get<int>();
        return Report(userInDb, power);

    } catch (std::invalid_argument e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("Error: json string with bad format: " + string(e.what()) + ", cannot create the Report");
    } catch (std::domain_error e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("Error: json string with bad format: missing fields, cannot create the Report");

    }

}

User ReportParserHTTP::getUserFromReportJson(json &json_content) {
    User requester;
    requester.id = json_content[REPORT_USER_ID].get<long>();
    requester.secretKey = json_content[REPORT_SECRET_KEY].get<std::string>();
    UserPreferenceProvider::checkValidUserInDB(requester);
    return requester;
}

long ReportParserHTTP::parseActiveRequest(std::string body) {
    try {
        json json_content = json::parse(body);
        User requester = getUserFromReportJson(json_content);
        UserPreferenceProvider::checkValidUserInDB(requester);

        User userInDb = UserPreferenceProvider::getUser(requester.id);
        userInDb.lastActivity = TimeUtility::getCurrentMillis();
        UserPreferenceProvider::updateUser(userInDb);

        return requester.id;
    } catch (std::invalid_argument e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("Error: json string with bad format: " + string(e.what()) + ", cannot create the active request");
    } catch (std::domain_error e) {
        syslog(LOG_INFO, e.what());
        throw std::invalid_argument("Error: json string with bad format: missing fields, cannot create the active request");

    }
}

