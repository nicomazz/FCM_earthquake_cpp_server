//
// Created by nicomazz97 on 20/11/16.
//

#include <Models/User/User.hpp>
#include <DataSources/UserPreferenceProvider.hpp>
#include <thread>
#include <Utility/UserMatching.hpp>
#include <client_http.hpp>
#include "FirebaseNotificationHandler.hpp"
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

void FirebaseNotificationHandler::handleEventNotification(Event e) {
    using namespace std::chrono;

    UserPreferenceProvider userPreferenceProvider;
    std::vector <User> toNotify = userPreferenceProvider.requestUsersToNotify(e);
    for (User user: toNotify){
        sendNotificationToUser(user,e);
        user.lastNotificationMillis =  duration_cast<milliseconds>
                (system_clock::now().time_since_epoch()).count();
        UserPreferenceProvider().updateUser(user);
    }
}

void FirebaseNotificationHandler::sendNotificationToUser(User user, Event e) {
    using namespace std;
    std::thread work_thread([&,&user, &e] {
        HttpClient client("https://fcm.googleapis.com");
        stringstream output;
        auto r=client.request("POST", "/fcm/send", FirebaseNotificationHandler::getJsonForUserEvent(user,e));
        output << r->content.rdbuf();
        cout<<"respose: "<<output.str()<<"\n";
       // assert(output.str()=="A string");
    });
    work_thread.detach();
}

std::string FirebaseNotificationHandler::getEventDetailsToSend(Event &e) {
    using namespace std;
    stringstream ss;
    ss<<e.eventLocation<<" ("<<e.magnitude<<") "<<e.date;
    return ss.str();
}

std::string FirebaseNotificationHandler::getJsonForUserEvent(User & user, Event &event){
    json j;
    /*
    "data": {
        "score": "5x1",
                "time": "15:10"
    },
    "to" : "bk3RNwTe3H0:CI2k_HHwgIpoDKCIZvvDMExUdFQ3P1..."*/

    j["data"] = getEventDetailsToSend(event);
    j["to"] = user.firebaseID;
    return j.dump();

}


