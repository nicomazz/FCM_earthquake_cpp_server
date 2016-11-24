//
// Created by nicomazz97 on 20/11/16.
//

#include "UserMatching.hpp"
#include "TimeUtility.hpp"
#include <chrono>
#include <iostream>


bool UserMatching::toNotify() {
    return respectMagnitudeConstraint() &&
           respectNotifyDelayConstraint() &&
           respectDistanceConstraint();
}

bool UserMatching::respectDistanceConstraint() {
    return getDistance() <= mUser.maxDistancePreference;
}
bool UserMatching::respectMagnitudeConstraint() {
    return mEvent.magnitude >= mUser.minMagPreference;
}

bool UserMatching::respectNotifyDelayConstraint() {
    using namespace std::chrono;
    long ms =  TimeUtils::getCurrentMillis();
    long dt = abs(ms - mUser.lastNotificationMillis);
    bool toNotify = dt > mUser.minMillisNotificationDelay;
    return toNotify;
}

// This function converts decimal degrees to radians
double UserMatching::deg2rad(double deg) {
    return (deg * PI / 180);
}

//  This function converts radians to decimal degrees
double UserMatching::rad2deg(double rad) {
    return (rad * 180 / PI);
}

double UserMatching::distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {

    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r) / 2);
    v = sin((lon2r - lon1r) / 2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));

}



double UserMatching::getDistance() {
    return distanceEarth(mEvent.lat,mEvent.lng, mUser.lat,mUser.lng);
}
