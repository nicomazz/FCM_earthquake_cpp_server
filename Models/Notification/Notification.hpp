//
// Created by nicomazz97 on 29/11/16.
//

#ifndef SIMPLE_WEB_SERVER_NOTIFICATION_H
#define SIMPLE_WEB_SERVER_NOTIFICATION_H

#pragma db object
class Notification {
public:
    Notification(){}

#pragma db id
    long id;
    int user_id;
    int event_id;
    long millis;

};


#endif //SIMPLE_WEB_SERVER_NOTIFICATION_H
