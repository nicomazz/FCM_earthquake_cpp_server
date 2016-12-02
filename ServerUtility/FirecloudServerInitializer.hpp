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
#include <Models/User/UserBuilder.h>
#include "Utility/json.hpp"
#include "../Detector/SimpleEQDetector.hpp"

namespace FCMServer {
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    using namespace std;
    using namespace boost::property_tree;


    void default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                               const shared_ptr<ifstream> &ifs);

    void initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server);
}
#endif //SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H
