//
// Created by nicomazz97 on 20/11/16.
//

#ifndef HTTPQUAKE_SERVER_H
#define HTTPQUAKE_SERVER_H

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
#include <Firebase/NotificationDataBuilder.hpp>
#include <Detector/ReportParserHTTP.hpp>
#include <DataSources/EventProvider.hpp>
#include <Models/Event/EventBuilder.hpp>
#include <Models/Report/DBReport.hpp>
#include <Models/Report/ReportBuilder.hpp>
#include <DataSources/ReportProvider.hpp>
#include "ServerUtility.hpp"
#include "WebCacher.hpp"

#include "HttpUserServer.hpp"
#include "HttpReportServer.hpp"


/**
 * All starts from initserver
 */
namespace HTTPQuakeServer {
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    typedef shared_ptr<HttpServer::Request> Request;
    typedef shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> Response;

    using namespace std;
    using namespace boost::property_tree;


    void initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server);





}
#endif //HTTPQUAKE_SERVER_H
