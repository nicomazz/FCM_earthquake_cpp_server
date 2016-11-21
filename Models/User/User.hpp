//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USER_H
#define SIMPLE_WEB_SERVER_USER_H

#include <odb/core.hxx>
#include <string>

#pragma db object
class User {
public:
    User(){}
#pragma db id auto
    long id;

    std::string firebaseID; // corrispondente al firebase id
    double lat;
    double lng;

    double minMagPreference;
    double maxDistancePreference;
    long minMillisNotificationDelay;

    long lastNotificationMillis;

    long lastModify;
};


#endif //SIMPLE_WEB_SERVER_USER_H
