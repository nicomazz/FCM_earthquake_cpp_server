//
// Created by nicomazz97 on 20/11/16.
//

#include <fstream>
#include <syslog.h>
#include <mutex>
#include "FirebaseNotificationManager.hpp"
#include "NotificationDataBuilder.hpp"

typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;

std::string FirebaseNotificationManager::firebase_key = "";


void FirebaseNotificationManager::handleEventNotification(Event e) {
    auto start = std::chrono::steady_clock::now();

    std::vector<User> toNotify = requestUsersToNotify(e);
    if (toNotify.size() == 0)
        return;

    for (User user: toNotify) {
        sendNotificationToUser(user, e);
        user.lastNotificationMillis = TimeUtility::getCurrentMillis();
        UserPreferenceProvider().updateUser(user);
    }

    auto end = std::chrono::steady_clock::now();
    int millis = (int)std::chrono::duration<double, std::milli>(end - start).count();

//    int totalUsers = (int) userProvider.requestUsersFromDB().size();
    syslog(LOG_INFO, "Time to send  notifications to %d users: %d milliseconds", (int)toNotify.size(), millis);
}

std::vector<User> FirebaseNotificationManager::requestUsersToNotify(Event event) {
    UserPreferenceProvider userProvider;
    //todo retrieve all users when server start, then update and not make new request every time
    std::vector<User> allUsers = userProvider.requestUsersFromDB();
    std::vector<User> toNotify;
    for (User &user: allUsers)
        if (isUserToBeNotified(user, event))
            toNotify.push_back(user);

    return toNotify;
}

void FirebaseNotificationManager::sendNotificationToUser(User user, Event e) {
    syslog(LOG_INFO, "Sending notification to user: %d", (int) user.id);
    try {
        static std::mutex send_mutex;
        std::lock_guard<std::mutex> guard(send_mutex);

        HttpsClient client("fcm.googleapis.com", false); //with false ignore certificate
        std::stringstream output;

        auto r = client.request("POST",
                                "/fcm/send",
                                NotificationDataBuilder::getJsonForUserEvent(user, e),
                                {{"Authorization", getFirebaseKey()},
                                 {"Content-Type",  "application/json"}});
        output << r->content.rdbuf();
        handleResults(user, output.str());
    } catch (std::invalid_argument e) { // no web connection
        syslog(LOG_INFO, e.what());
    }
}

void FirebaseNotificationManager::handleResults(User &user, std::string respose) {
    if (resultSucceded(respose)) return;

    if (userUnistalledApp(respose)) {
        removeUserFromDB(user);
        return;
    }
    std::string s = std::string("Problem in notification sending, respose: ") + respose;
    syslog(LOG_INFO, s.c_str());
}

bool FirebaseNotificationManager::isUserToBeNotified(User &u, Event &e) {
    return UserMatching(u, e).toNotify();
}

std::string FirebaseNotificationManager::getFirebaseKey() {
    if (firebase_key.size())
        return firebase_key;
    std::ifstream infile("secret_key.txt");
    if (!infile){
        std::ofstream outTest("here_must_be_the_secret_key!");
        syslog(LOG_INFO, "firebase key not found!");
        assert(firebase_key.size() > 0 && "firebase key not found!");
    }
    infile >> firebase_key;
    infile.close();
    return firebase_key;
}

bool FirebaseNotificationManager::resultSucceded(std::string respose) {
    return respose.find("\"success\":1") != std::string::npos;
}

bool FirebaseNotificationManager::userUnistalledApp(std::string respose) {
    return respose.find("NotRegistered") != std::string::npos;
}

void FirebaseNotificationManager::removeUserFromDB(User &u) {
    UserPreferenceProvider userProvider;
    long id = u.id;
    userProvider.removeUser(u);
    syslog(LOG_INFO, "Removed user with id: %d", (int) id);
}

