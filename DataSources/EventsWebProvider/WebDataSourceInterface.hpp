//
// Created by nicomazz97 on 17/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DATASOURCE_H
#define SIMPLE_WEB_SERVER_DATASOURCE_H

#include <Models/Event/Event.hpp>
#include <vector>
#include <sstream>
#include <ServerUtility/client_http.hpp>

class WebDataSourceInterface {
public:
    virtual std::vector<Event> requestEvents();

protected:
    virtual std::string getWebServiceUrl() = 0;

    virtual std::string getWebServiceUrlParams() = 0;

    virtual std::vector<Event> parseEvents(std::string webResponse) = 0;

    virtual Event buildEvent(std::string s) = 0;

    std::vector<std::string> split(std::string &str, char delimiter);

};

#endif //SIMPLE_WEB_SERVER_DATASOURCE_H
