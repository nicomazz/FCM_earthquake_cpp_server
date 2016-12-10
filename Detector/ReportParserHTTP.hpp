//
// Created by nicomazz97 on 01/12/16.
//

#ifndef SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
#define SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
#include <string>
#include "Report.hpp"


class ReportParserHTTP {
public:
    /**
     * Every report must have:
     * user
     * power (an integer)
     * @param body
     * @return
     */
    static Report parseRequest(std::string body);

    /**
     * message that the users send to notify their active status
     * @param body
     * @return
     */
    static long parseActiveRequest(std::string body);

private:

    static User getUserFromReportJson(json & body);
};
#endif //SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
