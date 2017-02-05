//
// Created by nicomazz97 on 05/02/17.
//

#include "HttpReportServer.hpp"

void HttpReportServer::initReportServer(SimpleWeb::Server<SimpleWeb::HTTP> &server) {
//get all report from to millis
    server.resource["^/reports/([0-9]+)/([0-9]+)$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                                       shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printReportsInInterval(request, response);
        });
        work_thread.detach();
    };

//get all report for a specific event id. (the parameters are event_id and millis, because the server
//could not have all the event in the db
//event id, millis,lat*10000,lng*10000
    server.resource["^/eventRelatedReports/([0-9]+)/([0-9]+)/([0-9]+)/([0-9]+)$"]["GET"] = [&server](
            shared_ptr<HttpServer::Response> response,
            shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printEventRelatedReports(request, response);
        });
        work_thread.detach();
    };

    server.resource["^/report"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                             shared_ptr<HttpServer::Request> request) {
        handleReport(request, response);
    };

    server.resource["^/detected_events"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                     shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printDetectedEvents(request, response);
        });
        work_thread.detach();
    };
//detected events from to millis
    server.resource["^/detected_events/([0-9]+)/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                                        shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printDetectedEventsFromToMillis(request, response);
        });
        work_thread.detach();
    };
}


void HttpReportServer::printReportsInInterval(Request request, Response response) {
    try {
        //todo fare questa cosa in modo più efficiente
        long from_millis = std::stol(request->path_match[1]);
        long to_millis = std::stol(request->path_match[2]);
        //stringstream ss;
        //ss << "from: " << from_millis << " to:" << to_millis;
        //syslog(LOG_INFO, ss.str().c_str());
        string res = getReportsFromToMillis(from_millis, to_millis);

        ServerUtils::outputHttpOKStringResponse(res, response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

string HttpReportServer::getReportsRelatedToEvent(Event &e) {
    vector<DBReport> res = ReportProvider::getReportsRelatedToEvents(e);
    return ReportBuilder::reportsToJson(res).dump(3);
}

string HttpReportServer::getReportsFromToMillis(long from_millis, long to_millis) {
    vector<DBReport> res = ReportProvider::getReportsFromToTime(from_millis, to_millis);
    return ReportBuilder::reportsToJson(res).dump(3);
}

void HttpReportServer::printEventRelatedReports(Request request, Response response) {
    try {
        //todo fare questa cosa in modo più efficiente
        Event e;
        e.id = std::stol(request->path_match[1]);
        e.millis = std::stol(request->path_match[2]);
        e.lat = std::stol(request->path_match[3]) / 10000;
        e.lng = std::stol(request->path_match[4]) / 10000;

        //caching response: if the server reboot we lose cached map, but we don't care
        static map<long, string> cached; //event_id, to_output
        string res = cached[e.id];

        // millis to recent or cached empty.
        if (res.size() == 0 || TimeUtility::getCurrentMillis() - e.millis < TimeUtility::MILLIS_IN_MINUTE * 5)
            res = cached[e.id] = getReportsRelatedToEvent(e);

        ServerUtils::outputHttpOKStringResponse(res, response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpReportServer::handleReport(HttpReportServer::Request request, HttpReportServer::Response response) {
    try {
        static SimpleEQDetector detector;
        string content = request->content.string();
        Report r = ReportParserHTTP::parseRequest(content);

        syslog(LOG_INFO, "New report received!");

        json json_resp;
        json_resp["respose"] = "Report send!";
        ServerUtils::outputHttpOKStringResponse(json_resp.dump(3), response);

        thread work_thread([response, r] {
            detector.addReport(r);
        });
        work_thread.detach();
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        ServerUtils::outputHttpBADStringResponse(resp, response);

    }
}


void HttpReportServer::printDetectedEvents(HttpReportServer::Request /*request*/, HttpReportServer::Response response) {
    try {
        static WebCacher detectedEventCacher(HttpReportServer::generateDetectedEvents,
                                             5000);
        ServerUtils::outputHttpOKStringResponse(detectedEventCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void
HttpReportServer::printDetectedEventsFromToMillis(HttpReportServer::Request request,
                                                  HttpReportServer::Response response) {
    try {
        long from = std::stol(request->path_match[1]);
        long to = std::stol(request->path_match[2]);

        string res = HttpReportServer::generateDetectedEventsFromToMillis(from, to);
        ServerUtils::outputHttpOKStringResponse(res, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

std::string HttpReportServer::generateDetectedEvents() {
    std::vector<Event> realTimeEvents = EventProvider::requestDetectedEvents();
    json jsonObj = json::array();

    for (Event &e : realTimeEvents)
        jsonObj.push_back(EventBuilder::eventToJson(e));

    return jsonObj.dump(3);
}

std::string HttpReportServer::generateDetectedEventsFromToMillis(long from, long to) {
    std::vector<Event> realTimeEvents = EventProvider::requestDetectedEvents(from, to);
    return EventBuilder::eventsToJson(realTimeEvents).dump(3);
}