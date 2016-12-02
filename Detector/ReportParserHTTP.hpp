//
// Created by nicomazz97 on 01/12/16.
//

#ifndef SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
#define SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
#include <string>
#include "Report.hpp"


class ReportParserHTTP {
public:
    static Report parseRequest(std::string body);
};
#endif //SIMPLE_WEB_SERVER_HTTPREQUESTPARSER_H
