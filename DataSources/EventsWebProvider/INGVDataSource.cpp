
#include <vector>
#include <ServerUtility/client_http.hpp>
#include <iostream>
#include <Utility/TimeUtility.hpp>
#include "INGVDataSource.hpp"


std::vector<Event> INGVDataSource::parseEvents(std::string webResponse) {
    std::vector<std::string> eventsStrings = split(webResponse, '\n');
    std::vector<Event> result;
    for (std::string eStr : eventsStrings) {
        try {
            Event event = buildEvent(eStr);
            result.push_back(event);
        } catch (...) {
            std::cerr << "eccezione nel parse di un'evento\n";
        }
    }
    return result;
}

//esempio di stringa:
//10064391|2016-11-17T00:13:15.720000|43.0077|13.1222|9.4|SURVEY-INGV||||ML|3.2|--|Macerata
//#EventID|Time|Latitude|Longitude|Depth/km|Author|Catalog|Contributor|ContributorID|MagType|Magnitude|MagAuthor|EventLocationName

Event INGVDataSource::buildEvent(std::string s) {
    std::vector<std::string> splitted = split(s, '|');
    Event event;
    std::stringstream ss;
    std::string id;
    id = splitted[0];
    event.date = splitted[1];
    ss = std::stringstream(splitted[2] + " " + splitted[3] + " " + splitted[4]);
    ss >> event.lat >> event.lng >> event.depthKm;
    event.author = splitted[5];
    event.catalog = splitted[6];
    event.contributor = splitted[7];
    event.contributorId = splitted[8];
    event.magType = splitted[9];
    ss = std::stringstream(splitted[10]);
    ss >> event.magnitude;
    event.author = splitted[11];
    event.eventLocation = splitted[12];

    //if the id is a string we get it hashcode (like in java)
    try { event.id = stol(id); }
    catch (std::invalid_argument e) { event.id = hashCode(id); }


    //if (event.author.size() == 0)
    event.author = getDataSourceName();
    event.millis = TimeUtility::getMillisFromTimeString(event.date);
    return event;
}

std::string INGVDataSource::getWebServiceUrlParams() {
    return "/fdsnws/event/1/query?format=text";
}

std::string INGVDataSource::getWebServiceUrl() {
    return "webservices.ingv.it";
}

std::string INGVDataSource::getDataSourceName() {
    return "INGV";
}
