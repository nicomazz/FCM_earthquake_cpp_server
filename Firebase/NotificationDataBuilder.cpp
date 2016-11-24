#include "NotificationDataBuilder.h"
#include "../Utility/json.hpp"

std::string NotificationDataBuilder::getJsonForUserEvent(User user, Event e){
	json j;
	j["data"] = getJsonDetails(e);
	j["to"] = user.firebaseID;
	return j.dump();
}

json NotificationDataBuilder::getJsonDetails(Event e){
	json data;
	data["details"] = e.eventDetails....
	//todo add other details..
	return data;
}