
#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event{
public:
	Event(){}
	Event(int id, std::string location);
	int id;
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
};

#endif
