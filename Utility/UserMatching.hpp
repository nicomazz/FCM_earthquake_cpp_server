//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERMATCHING_H
#define SIMPLE_WEB_SERVER_USERMATCHING_H


#include <Models/User/User.hpp>
#include <Models/Event/Event.hpp>


#include <cmath>


class UserMatching {
public:

    UserMatching(User aUser, Event aEvent) :
            mUser(aUser),
            mEvent(aEvent) {}

    bool toNotify();

private:
    User mUser;
    Event mEvent;

    double getDistance();

    bool respectDistanceConstraint();

    bool respectMagnitudeConstraint();

    bool respectNotifyDelayConstraint();

    bool respectRealtimeReport();
};


#endif //SIMPLE_WEB_SERVER_USERMATCHING_H
