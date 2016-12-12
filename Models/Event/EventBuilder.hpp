//
// Created by nicomazz97 on 11/12/16.
//

#ifndef SIMPLE_WEB_SERVER_EVENTBUILDER_HPP
#define SIMPLE_WEB_SERVER_EVENTBUILDER_HPP


#include <Firebase/FirebaseNotificationManager.hpp>
#include <Models/User/User.hpp>
#include "event_field.hpp"

class EventBuilder {

public:

    static json eventToJson(Event &e);

};


#endif //SIMPLE_WEB_SERVER_EVENTBUILDER_HPP
