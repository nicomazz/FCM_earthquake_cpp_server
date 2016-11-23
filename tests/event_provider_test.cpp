
//
// Created by nicomazz97 on 21/11/16.
//

#include "server_http.hpp"
#include "client_http.hpp"
#include "FirecloudServerInitializer.h"

#include <cassert>

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

int main() {
    HttpServer server(8080, 1);

    FCMServer::initServer(server);

    thread server_thread([&server](){
        //Start server
        server.start();
    });

    EventProvider ep;

    // test event add/delete

    //persist
    Event e1 = Event();
    assert(e1.id <= 0);
    e1.eventLocation = "testEvent";
    long inserted = ep.persist(e1);
    assert(e1.id > 0);
    assert(inserted > 0);

    //ispresent
    assert(ep.isEventPresent(e1.id));

    //delete
    ep.deleteEvent(e1);
    Event deleted = ep.getEvent(e1.id);
    assert(deleted.id<=0);


    //todo
    server.stop();
    server_thread.join();

    return 0;
}
