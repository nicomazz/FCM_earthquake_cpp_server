//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H
#define SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <Models/User/User.hpp>
#include <DataSources/UserPreferenceProvider.hpp>
#include <Models/User/UserBuilder.hpp>
#include "Utility/json.hpp"
#include "../Detector/SimpleEQDetector.hpp"

namespace FCMServer {
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    typedef shared_ptr<HttpServer::Request> Request;
    typedef shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> Response;

    using namespace std;
    using namespace boost::property_tree;


    void initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server);

    void handleUserRequest(Request request, Response response);

    void printAllUsers(Response response);

    //id in url of request
    void printUserWithId(Request request, Response response);

    void printDetectedEvents(Request request, Response response);

    void printDetectedEventsFromToMillis(Request request, Response response);

    /**
     * read from request a json array of ids, and return a list of requested users.
     * The request must be made by an user that provide a secret key
     * @param request
     * @param response
     */
    void getUsersDetails(Request request, Response response);

    void handleReport(Request request, Response response);


    void handleUserActivity(Request request, Response response);


    void getActiveUsers(Request request, Response response);

    void getRecentUsers(Request request, Response response);

    string getReportsRelatedToEvent(Event &e);

    string getReportsFromToMillis(long from_millis, long to_millis);

    void printReportsInInterval(Request request, Response response);


    void printEventRelatedReports(Request request, Response response);


}
#endif //SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H
