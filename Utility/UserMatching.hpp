//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERMATCHING_H
#define SIMPLE_WEB_SERVER_USERMATCHING_H


#include <Models/User/User.hpp>
#include <Models/Event/Event.hpp>


#include <cmath>

#define PI 3.14159265358979323846
#define earthRadiusKm 6371.0


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


    double deg2rad(double deg);

    double rad2deg(double rad);

    /**
    * Returns the distance between two points on the Earth.
    * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
    * @param lat1d Latitude of the first point in degrees
    * @param lon1d Longitude of the first point in degrees
    * @param lat2d Latitude of the second point in degrees
    * @param lon2d Longitude of the second point in degrees
    * @return The distance between the two points in kilometers
    */
    double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
};


#endif //SIMPLE_WEB_SERVER_USERMATCHING_H
