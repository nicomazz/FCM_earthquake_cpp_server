//
// Created by nicomazz97 on 05/02/17.
//

#include "HttpReportServer.hpp"
#include "ReportChecker.hpp"

void HttpEventReportServer::initReportServer(SimpleWeb::Server<SimpleWeb::HTTP> &server) {
//get all report from to millis
    server.resource["^/reports/([0-9]+)/([0-9]+)$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                                       shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printReportsInMillisInterval(request, response);
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
    /**
     * download events from datasource and try to set numberOFReports to each one.
     * the parameter is a millisecond value. Will be parsed the events in that day.
     */
    server.resource["^/parse_events/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                            shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            downloadAndPrintEventsInDayMillis(request, response);
        });
        work_thread.detach();
    };
    //from millis, to millis
    server.resource["^/parse_events/([0-9]+)/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                                     shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            downloadAndPrintEventsInDateRange(request, response);
        });
        work_thread.detach();
    };
}


void HttpEventReportServer::printReportsInMillisInterval(Request request, Response response) {
    try {
        long from_millis = std::stol(request->path_match[1]);
        long to_millis = std::stol(request->path_match[2]);
        //stringstream ss;
        //ss << "from: " << from_millis << " to:" << to_millis;
        //syslog(LOG_INFO, ss.str().c_str());
        //todo fare questa cosa in modo più efficiente

        string res = getReportsFromToMillis(from_millis, to_millis);

        ServerUtils::outputHttpOKStringResponse(res, response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

string HttpEventReportServer::getReportsRelatedToEvent(Event &e) {
    vector<DBReport> res = ReportProvider::getReportsRelatedToEvents(e);
    return ReportBuilder::reportsToJson(res).dump(3);
}

string HttpEventReportServer::getReportsFromToMillis(long from_millis, long to_millis) {
    vector<DBReport> res = ReportProvider::getReportsFromToTime(from_millis, to_millis);
    return ReportBuilder::reportsToJson(res).dump(3);
}

void HttpEventReportServer::printEventRelatedReports(Request request, Response response) {
    try {
        //todo fare questa cosa in modo più efficiente
        Event e;
        e.id = std::stol(request->path_match[1]);
        e.millis = std::stol(request->path_match[2]);
        e.lat = ((double) std::stol(request->path_match[3])) / 10000;
        e.lng = ((double) std::stol(request->path_match[4])) / 10000;

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

void
HttpEventReportServer::handleReport(HttpEventReportServer::Request request, HttpEventReportServer::Response response) {
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


void HttpEventReportServer::printDetectedEvents(HttpEventReportServer::Request /*request*/,
                                                HttpEventReportServer::Response response) {
    try {
        static WebCacher detectedEventCacher(HttpEventReportServer::generateDetectedEvents,
                                             5000);
        ServerUtils::outputHttpOKStringResponse(detectedEventCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void
HttpEventReportServer::printDetectedEventsFromToMillis(HttpEventReportServer::Request request,
                                                       HttpEventReportServer::Response response) {
    try {
        long from = std::stol(request->path_match[1]);
        long to = std::stol(request->path_match[2]);

        string res = HttpEventReportServer::generateDetectedEventsFromToMillis(from, to);
        ServerUtils::outputHttpOKStringResponse(res, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpEventReportServer::downloadAndPrintEventsInDayMillis(Request request, Response response) {
    try {
        long millis = std::stol(request->path_match[1]);
        millis = TimeUtility::getPrecMidnightMillis(millis);

        string res = generateEventAndUpdateReportsFromWeb(millis, millis);

        ServerUtils::outputHttpOKStringResponse(res, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpEventReportServer::downloadAndPrintEventsInDateRange(Request request, Response response) {
    try {
        long from_millis = std::stol(request->path_match[1]);
        long to_millis = std::stol(request->path_match[2]);

        from_millis = TimeUtility::getPrecMidnightMillis(from_millis);
        to_millis = TimeUtility::getNextMidnightMillis(to_millis);

        string res = generateEventAndUpdateReportsFromWeb(from_millis, to_millis);
        ServerUtils::outputHttpOKStringResponse(res, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

std::string HttpEventReportServer::generateDetectedEvents() {
    std::vector<Event> realTimeEvents = EventProvider::requestDetectedEvents();
    json jsonObj = json::array();

    for (Event &e : realTimeEvents)
        jsonObj.push_back(EventBuilder::eventToJson(e));

    return jsonObj.dump(3);
}

std::string HttpEventReportServer::generateDetectedEventsFromToMillis(long from, long to) {
    std::vector<Event> realTimeEvents = EventProvider::requestDetectedEvents(from, to);
    return EventBuilder::eventsToJson(realTimeEvents).dump(3);
}

std::string HttpEventReportServer::generateEventAndUpdateReportsFromWeb(long from, long to) {
    std::vector<Event> allEventsParsed = EventProvider::requestAndPersistEventFromWebInDateRange(from, to);
    for (Event e: allEventsParsed)
        ReportChecker::updateEventReportsNumber(e);
    std::string res = EventBuilder::eventsToJson(allEventsParsed).dump(3);
    EventProvider::removeOldEvents();
    return res;
}




