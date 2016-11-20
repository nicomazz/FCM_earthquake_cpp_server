//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H
#define SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H


#include <Models/Event/Event.hpp>
#include "../Utility/json.hpp"

// for convenience
using json = nlohmann::json;

class FirebaseNotificationHandler {
public:
    void handleEventNotification(Event e);

private:
    void sendNotificationToUser(User user, Event e);

    std::string getJsonForUserEvent(User &user, Event &event);

    std::string getEventDetailsToSend(Event & e);
};


#endif //SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H
