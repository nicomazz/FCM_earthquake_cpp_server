//
// Created by nicomazz97 on 05/02/17.
//

#ifndef SIMPLE_WEB_SERVER_HTTPUSERSERVER_HPP
#define SIMPLE_WEB_SERVER_HTTPUSERSERVER_HPP


#include "server_http.hpp"
#include "client_http.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

#include <Models/User/User.hpp>
#include <DataSources/UserPreferenceProvider.hpp>
#include <Models/User/UserBuilder.hpp>
#include "Utility/json.hpp"
#include "../Detector/SimpleEQDetector.hpp"
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <Models/User/User.hpp>
#include <DataSources/UserPreferenceProvider.hpp>
#include <Models/User/UserBuilder.hpp>
#include "Utility/json.hpp"
#include "../Detector/SimpleEQDetector.hpp"
#include <Firebase/NotificationDataBuilder.hpp>
#include <Detector/ReportParserHTTP.hpp>
#include <DataSources/EventProvider.hpp>
#include <Models/Event/EventBuilder.hpp>
#include <Models/Report/DBReport.hpp>
#include <Models/Report/ReportBuilder.hpp>
#include <DataSources/ReportProvider.hpp>
#include "ServerUtility.hpp"
#include "WebCacher.hpp"


namespace HttpUserServer {

    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    typedef shared_ptr<HttpServer::Request> Request;
    typedef shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> Response;

    using namespace std;
    using namespace boost::property_tree;


    void initUserFunctions(SimpleWeb::Server<SimpleWeb::HTTP> &server);

    void handleUserRequest(Request request, Response response);

    void printUserWithId(Request request, Response response);

    void printAllUsers(Response response);

    void handleUserActivity(Request request, Response response);

    void getActiveUsers(Request request, Response response);

    void getRecentUsers(Request request, Response response);

    /**
    * read from request a json array of ids, and return a list of requested users.
    * The request must be made by an user that provide a secret key
    * @param request
    * @param response
    */
    void getUsersDetails(Request request, Response response);


    std::string generateAllUsers();

    std::string generateActiveUsers();

    std::string generateRecentUsers();
};


#endif //SIMPLE_WEB_SERVER_HTTPUSERSERVER_HPP
