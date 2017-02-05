//
// Created by nicomazz97 on 05/02/17.
//

#ifndef SIMPLE_WEB_SERVER_HTTPREPORTSERVER_HPP
#define SIMPLE_WEB_SERVER_HTTPREPORTSERVER_HPP

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


namespace HttpReportServer {
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    typedef shared_ptr<HttpServer::Request> Request;
    typedef shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> Response;

    using namespace std;
    using namespace boost::property_tree;

    void initReportServer(SimpleWeb::Server<SimpleWeb::HTTP> &server);

    //id in url of request

    void printDetectedEvents(Request request, Response response);

    void printDetectedEventsFromToMillis(Request request, Response response);



    void handleReport(Request request, Response response);


    string getReportsRelatedToEvent(Event &e);

    string getReportsFromToMillis(long from_millis, long to_millis);

    void printReportsInInterval(Request request, Response response);


    void printEventRelatedReports(Request request, Response response);

    std::string generateDetectedEvents();

    std::string generateDetectedEventsFromToMillis(long from, long to);

};


#endif //SIMPLE_WEB_SERVER_HTTPREPORTSERVER_HPP
