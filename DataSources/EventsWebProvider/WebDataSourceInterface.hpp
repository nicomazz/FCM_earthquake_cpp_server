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
    /**
     * parse all the events in the millis's day
     * @param millis: if millis is currentmillis parse all events from last midnight to this midnight
     * @return
     */
    virtual std::vector<Event> requestEventsInADay(long millis);

    virtual std::vector<Event> requestEventsInDateRange(long from, long to);

    virtual std::string getDataSourceName() = 0;

    static std::string getUrlParameterForDay(long millis);
protected:

    virtual std::string getWebServiceUrl() = 0;
    virtual std::string getWebServiceUrlParams() = 0;
    static std::string getUrlParametersForDateRange(long from, long to);


    virtual std::vector<Event> parseEvents(std::string webResponse) = 0;

    virtual Event buildEvent(std::string s) = 0;

    std::vector<std::string> split(std::string &str, char delimiter);

    static int hashCode(std::string & s);

//private:
};

#endif //SIMPLE_WEB_SERVER_DATASOURCE_H
