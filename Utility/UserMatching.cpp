//
// Created by nicomazz97 on 20/11/16.
//

#include "UserMatching.hpp"
#include "TimeUtility.hpp"
#include "GeoUtility.hpp"
#include <chrono>
#include <iostream>


bool UserMatching::toNotify() {
    if (mEvent.isRealTimeReport)
        return respectRealtimeReport();

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
    long ms = TimeUtility::getCurrentMillis();
    long dt = abs(ms - mUser.lastNotificationMillis);
    bool toNotify = dt > mUser.minMillisNotificationDelay;
    return toNotify;
}


double UserMatching::getDistance() {
    return GeoUtility::distanceEarth(mEvent.lat, mEvent.lng, mUser.lat, mUser.lng);
}

bool UserMatching::respectRealtimeReport() {
    if (!mEvent.isRealTimeReport) return true;
    return mUser.receiveRealTimeNotification;
}
