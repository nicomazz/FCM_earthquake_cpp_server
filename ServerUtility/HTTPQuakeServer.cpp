//
// Created by nicomazz97 on 26/11/16.
//


#include "HTTPQuakeServer.hpp"

void HTTPQuakeServer::initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server) {

    HttpUserServer::initUserFunctions(server);
    HttpEventReportServer::initReportServer(server);
    //Default GET-example. If no other matches, this anonymous function will be called.
    //Will respond with content in the web/-directory, and its subdirectories.
    //Default file: index.html
    //Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    server.default_resource["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                               shared_ptr<HttpServer::Request> /*request*/) {
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << 0 << "\r\n\r\n";
        //here was the code to generate a simple html page from a file
        return;
    };
}


