
#ifndef EVENT_H
#define EVENT_H

#include <string>

#include <odb/core.hxx>

#pragma db object
class Event{
public:
    Event(){id = 0;}
	Event(long id, std::string location);

    #pragma db id
	long id;
	std::string date;
	double depthKm;
	std::string author;
	std::string catalog; 
	std::string contributor;
	std::string contributorId;
	std::string magType;
	double magnitude;
	std::string magAuthor;
	std::string eventLocation;
	long millis;
	double distance;
	double lat;
	double lng;
private:
    friend class odb::access;

};

#endif
