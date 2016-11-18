
#include <vector>
#include <client_http.hpp>
#include <iostream>
#include "INGVDataSource.hpp"



std::vector <Event> INGVDataSource::parseEvents(std::string webResponse) {
    std::vector<std::string> eventsStrings = split(webResponse,'\n');
    std::vector<Event> result;
    for (std::string eStr : eventsStrings){
        try{
            Event event = buildEvent(eStr);
            result.push_back(event);
        } catch (...){
            std::cerr<<"eccezione nel parse di un'evento\n";
        }
    }
    return result;
}
//esempio di stringa:
//10064391|2016-11-17T00:13:15.720000|43.0077|13.1222|9.4|SURVEY-INGV||||ML|3.2|--|Macerata
Event INGVDataSource::buildEvent(std::string s) {
    std::vector<std::string> splitted = split(s,'|');
    std::string toParse;
    for (std::string component: splitted){
        if (component.size())
            toParse+= component+" ";
        else toParse+= "- ";
    }
    Event event;
    std::stringstream ss(toParse);
    ss>>event.id>>event.date>>event.lat>>event.lng;
    ss>>event.depthKm>>event.author>>event.catalog>>event.contributor>>event.contributorId>>event.magType;
    ss>>event.magnitude>>event.author>>event.eventLocation;
    return event;
}

std::string INGVDataSource::getWebServiceUrlParams() {
    return "/fdsnws/event/1/query?format=text";
}
std::string INGVDataSource::getWebServiceUrl() {
    return "webservices.ingv.it";
}