#include <Models/User/User.hpp>
#include <Models/Event/Event.hpp>
#include <Models/User/UserBuilder.h>
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
	data["details"] = e.eventLocation+ " " + e.date;
	//todo add other details..
	return data;
}