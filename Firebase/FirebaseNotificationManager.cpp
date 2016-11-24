//
// Created by nicomazz97 on 20/11/16.
//

#include "FirebaseNotificationManager.hpp"

typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;

std::string FirebaseNotificationManager::firebase_key = "";


void FirebaseNotificationManager::handleEventNotification(Event e) {

    std::vector<User> toNotify = requestUsersToNotify(e);
    std::cout<<"Number of user to be notified: "<<toNotify.size()<<"\n";
    for (User user: toNotify) {
        sendNotificationToUser(user, e);
        user.lastNotificationMillis = TimeUtils::getCurrentMillis();
        UserPreferenceProvider().updateUser(user);
    }
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
    //todo verificare se eseguire sul main thread 
 //   std::thread work_thread([&, &user, &e] {
        std::cout<<"sending notification at user id: "<<user.id<<std::endl;
        HttpsClient client("fcm.googleapis.com",false); //with false ignore certificate
        std::stringstream output;

        auto r = client.request("POST",
            "/fcm/send",
            NotificationDataBuilder::getJsonForUserEvent(user, e),
            {{"Authorization",getFirebaseKey()},
            {"Content-Type","application/json"}});
        output << r->content.rdbuf();
        controlEsit(output.str());

        // assert(output.str()=="A string");
   // });
  //  work_thread.detach();
    }

    void FirebaseNotificationManager::contolEsit(std::string respose){
        std::cout << "respose: " << output.str() << "\n";
    }


//todo rename this
    bool FirebaseNotificationManager::isUserToBeNotified(User &u, Event &e) {
        return UserMatching(u, e).toNotify();
    }

    std::string FirebaseNotificationManager::getFirebaseKey(){
        if (firebase_key.size()) return firebase_key;
        std::ifstream infile("secret_key.txt");
        infile>>firebase_key;
        assert(firebase_key.size() > 0,"firebase key not present!");
        infile.close();
        return firebase_key;
    }