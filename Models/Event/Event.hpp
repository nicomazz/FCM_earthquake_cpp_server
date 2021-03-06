
#ifndef EVENT_H
#define EVENT_H

#include <string>

#include <odb/core.hxx>

#pragma db object

class Event {
public:
    Event() {
        id = millis = 0;
        numberOfReports = 0;
        isRealTimeReport = false;
        magnitude = 3;
    }

    Event(long id, std::string location);

#pragma db id
    long id;
    std::string date;
    double depthKm;
    std::string author;
    std::string catalog;
    std::string contributor;
    std::string contributorId; // for generated events contain a json array of id's "[1,2,3,4]
    std::string magType;
    double magnitude;
    std::string magAuthor;
    std::string eventLocation;
    long millis;
    double lat;
    double lng;
    bool isRealTimeReport;
    int numberOfReports;
private:
    friend class odb::access;

};

#endif
