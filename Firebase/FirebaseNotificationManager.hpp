//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H
#define SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H


#include "Models/Event/Event.hpp"
#include "Utility/json.hpp"
#include "Utility/TimeUtility.hpp"
#include "Models/User/User.hpp"
#include "DataSources/UserPreferenceProvider.hpp"
#include "Utility/UserMatching.hpp"
#include "client_https.hpp"

#include <thread>

// for convenience
using json = nlohmann::json;

class FirebaseNotificationManager {
public:
    void handleEventNotification(Event e);

    std::vector<User> requestUsersToNotify(Event e);

private:
    void sendNotificationToUser(User user, Event e);

    std::string getJsonForUserEvent(User &user, Event &event);

    std::string getEventDetailsToSend(Event &e);

    bool isUserToBeNotified(User &u, Event &e);


};


#endif //SIMPLE_WEB_SERVER_FIREBASENOTIFICATIONHANDLER_H
