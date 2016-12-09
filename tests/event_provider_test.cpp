
//
// Created by nicomazz97 on 21/11/16.
//

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "ServerUtility/ServerInitializer.hpp"
#include "../DataSources/EventProvider.hpp"

#include <cassert>

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

int main() {

    EventProvider ep;

    // test event add/delete

    //persist
    Event e1 = Event();
    assert(e1.id <= 0);
    e1.eventLocation = "testEvent";
    e1.id = TimeUtils::getCurrentMillis();

    int pre_size = (int) ep.requestEventFromDB().size();
    long inserted = ep.persistEvent(e1);
    assert(e1.id > 0);
    assert(inserted >= 0);
    assert((int) ep.requestEventFromDB().size() == pre_size + 1);

    //ispresent
    assert(ep.isEventPresent(e1.id));

    //delete
    ep.deleteEvent(e1);
    Event deleted = ep.getEvent(e1.id);
    assert(deleted.id<=0);


    std::cerr<<"All is working For Event!";

    return 0;
}
