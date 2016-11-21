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

    //todo
    server.stop();
    server_thread.join();

    return 0;
}
