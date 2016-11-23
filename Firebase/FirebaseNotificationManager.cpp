//
// Created by nicomazz97 on 20/11/16.
//

#include "FirebaseNotificationManager.hpp"
#include "secretKey.h"

#ifndef SECRET_KEY
#define SECRET_KEY "secret key must be provided by firebase!"
#endif

typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;

void FirebaseNotificationManager::handleEventNotification(Event e) {

    std::vector<User> toNotify = requestUsersToNotify(e);
    std::cout<<"Number of user to be notified: "<<toNotify.size()<<"\n";
    for (User user: toNotify) {
        sendNotificationToUser(user, e);
        user.lastNotificationMillis = TimeUtils::getCurrentMillis();
        UserPreferenceProvider().updateUser(user);
    }
}

void FirebaseNotificationManager::sendNotificationToUser(User user, Event e) {
    std::thread work_thread([&, &user, &e] {
        std::cout<<"sending notification at user id: "<<user.id<<std::endl;
        HttpsClient client("fcm.googleapis.com",false); //with false ignore certificate
        std::stringstream output;

        auto r = client.request("POST",
                                "/fcm/send",
                                FirebaseNotificationManager::getJsonForUserEvent(user, e),
                                {{"Authorization",SECRET_KEY},
                                 {"Content-Type","application/json"}});
        output << r->content.rdbuf();
        std::cout << "respose: " << output.str() << "\n";
        // assert(output.str()=="A string");
    });
    work_thread.detach();
}

std::string FirebaseNotificationManager::getEventDetailsToSend(Event &e) {
    using namespace std;
    stringstream ss;
    ss << e.eventLocation << " (" << e.magnitude << ") " << e.date;
    return ss.str();
}

std::string FirebaseNotificationManager::getJsonForUserEvent(User &user, Event &event) {
    json j;
    json data;
    data["details"] = getEventDetailsToSend(event);
    j["data"] = data;
    j["to"] = user.firebaseID;
    std::cout<<"dump json: "<<j.dump(3)<<"\n";
    return j.dump();
}

std::vector<User> FirebaseNotificationManager::requestUsersToNotify(Event event) {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestUsersFromDB();
    std::vector<User> toNotify;

    for (User &user: allUsers)
        if (isUserToBeNotified(user, event)) {
            toNotify.push_back(user);
        }

    return toNotify;
}
bool FirebaseNotificationManager::isUserToBeNotified(User &u, Event &e) {
    return UserMatching(u, e).toNotify();
}