//
// Created by nicomazz97 on 18/11/16.
//
#include <iostream>
#include <syslog.h>
#include <Utility/TimeUtility.hpp>
#include "WebDataSourceInterface.hpp"

std::vector<std::string> WebDataSourceInterface::split(std::string &str, char delimiter) {
    static char buffer[5000];
    std::vector<std::string> result;
    int sz = str.size();
    int ind = 0;
    for (int i = 0; i < sz; i++) {

        if (str[i] != delimiter) {
            buffer[ind++] = str[i];
        } else {
            buffer[ind] = 0;
            result.push_back(std::string(buffer));
            ind = 0;
        }
    }
    buffer[ind] = 0;
    result.push_back(std::string(buffer));
    return result;
}

std::vector<Event> WebDataSourceInterface::requestEvents() {
    return requestEventsInADay(TimeUtility::getCurrentMillis());
}

std::string WebDataSourceInterface::getUrlParameterForDay(long millis) {
    return getUrlParametersForDateRange(millis, millis);
}

std::string WebDataSourceInterface::getUrlParametersForDateRange(long from, long to) {
    from = TimeUtility::getPrecMidnightMillis(from);
    to = TimeUtility::getNextMidnightMillis(to);

    std::string midnightString = TimeUtility::getTimeStringFromMillis(from);
    std::string toNextMidnight = TimeUtility::getTimeStringFromMillis(to);
    std::stringstream ss;
    ss << "&starttime=" << midnightString << "&endtime=" << toNextMidnight;
    return ss.str();
}

std::vector<Event> WebDataSourceInterface::requestEventsInADay(long millis) {
    return requestEventsInDateRange(millis, millis);
}

std::vector<Event> WebDataSourceInterface::requestEventsInDateRange(long from, long to) {
    try {
        SimpleWeb::Client<SimpleWeb::HTTP> client(getWebServiceUrl());
        std::string respose;
        {
            std::stringstream output;
            auto r = client.request("GET", getWebServiceUrlParams() + getUrlParametersForDateRange(from, to));
            output << r->content.rdbuf();
            respose = output.str();
        }
        // todo check if it is a bad respose
        return parseEvents(respose);
    } catch (std::invalid_argument e) { // no web connection
        syslog(LOG_INFO, e.what());
        return std::vector<Event>();
    }
}

int WebDataSourceInterface::hashCode(std::string &s) {
    int h = 0;
    for (char c : s)
        h = 31 * h + c;
    if (h < 0) h *= -1;
    return h;
}

