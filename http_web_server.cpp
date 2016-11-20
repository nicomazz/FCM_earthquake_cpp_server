#include "server_http.hpp"
#include "client_http.hpp"
#include "DataSources/EventProvider.hpp"
#include "FirecloudServerInitializer.h"

//Added for the json-example
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define BOOST_SPIRIT_THREADSAFE

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
//Added for the default_resource example
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <DataSources/UserPreferenceProvider.hpp>
#include <Firebase/FirebaseNotificationHandler.hpp>

using namespace std;
//Added for the json-example:
using namespace boost::property_tree;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

//Added for the default_resource example
void default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                           const shared_ptr<ifstream> &ifs);

void wait(int seconds) {
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}


void eventsParse() {
    EventProvider dataSource;
    FirebaseNotificationHandler notificationHandler;
    for (;;) {
        wait(30);
        std::vector<Event> newEvents = dataSource.requestNewEventNotInDB();
        std::cout << "numero di nuovi eventi non presenti prima: " << newEvents.size() << "\n";
        for (Event &e: newEvents) {
            dataSource.persistEvent(e);
            notificationHandler.handleEventNotification(e);
        }

        std::vector<Event> eventsInDb = dataSource.requestEventFromDB();
        std::cout << "eventi nel db attualmente: " << eventsInDb.size() << "\n";
        std::cout << "esecuzione attuale terminata\n\n\n";
    }
}



int main() {
    //HTTP-server at port 8080 using 1 thread
    //Unless you do more heavy non-threaded processing in the resources,
    //1 thread is usually faster than several threads
    HttpServer server(8080, 2);

    FCMServer::initServer(server);

    thread server_thread([&server]() {
        //Start server
        server.start();
    });

    //Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    //Client examples
    HttpClient client("localhost:8080");
    auto r1 = client.request("GET", "/match/123");
    cout << r1->content.rdbuf() << endl;

    string json_string = "{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";
    auto r2 = client.request("POST", "/string", json_string);
    cout << r2->content.rdbuf() << endl;

    auto r3 = client.request("POST", "/json", json_string);
    cout << r3->content.rdbuf() << endl;

    boost::thread t{eventsParse};
    t.join();

    server_thread.join();

    return 0;
}


#pragma clang diagnostic pop