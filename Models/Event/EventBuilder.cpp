//
// Created by nicomazz97 on 11/12/16.
//

#include "EventBuilder.hpp"

json EventBuilder::eventToJson(Event &e){
    json json_content;
    json_content[EVENT_ID] = e.id;
    json_content[EVENT_DATE] = e.date;
    json_content[EVENT_DEPTH] = e.depthKm;
    json_content[EVENT_AUTHOR] = e.author;
    json_content[EVENT_CONTRIBUTOR_ID] = e.contributorId;
    json_content[EVENT_MAG_TYPE] = e.magType;
    json_content[EVENT_MAGNITUDE] = e.magnitude;
    json_content[EVENT_LOCATION] = e.eventLocation;
    json_content[EVENT_MILLIS] = e.millis;
    json_content[EVENT_LAT] = e.lat;
    json_content[EVENT_LNG] = e.lng;
    json_content[EVENT_REAL_TIME] = e.isRealTimeReport;
    json_content[EVENT_REPORTS_NUMBER] = e.numberOfReports;
    return json_content;
}

json EventBuilder::eventsToJson(std::vector<Event> &v) {
    json res = json::array();
    for (Event & e : v)
        res.push_back(eventToJson(e));
    return res;
}
