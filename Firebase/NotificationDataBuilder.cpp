#include <Models/User/User.hpp>
#include <Models/Event/Event.hpp>
#include <Models/User/UserBuilder.hpp>
#include "NotificationDataBuilder.hpp"
#include "../Utility/json.hpp"

std::string NotificationDataBuilder::getJsonForUserEvent(User &user, Event &e){
	json j;
	j["data"] = getJsonDetails(e);
	j["to"] = user.firebaseID;
	return j.dump();
}

json NotificationDataBuilder::getJsonDetails(Event e){
	json data;
	data["realTime"] = e.isRealTimeReport;
	data["location"] = e.eventLocation;
	data["dateTime"] = e.date;
	data["mag"] = e.magnitude;
	data["lat"] = e.lat;
	data["lng"] = e.lng;
	//todo add other details..
	return data;
}