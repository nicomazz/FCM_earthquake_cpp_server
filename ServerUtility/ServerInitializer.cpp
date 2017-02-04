//
// Created by nicomazz97 on 26/11/16.
//

#include <Firebase/NotificationDataBuilder.hpp>
#include <Detector/ReportParserHTTP.hpp>
#include <DataSources/EventProvider.hpp>
#include <Models/Event/EventBuilder.hpp>
#include <Models/Report/DBReport.hpp>
#include <Models/Report/ReportBuilder.hpp>
#include <DataSources/ReportProvider.hpp>
#include "ServerInitializer.hpp"
#include "ServerFunctions.hpp"
#include "WebCacher.hpp"

// 5 min

void FCMServer::initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server) {

    /**
     * handle request to add/update user info.
     * the respose is a json with the id of the new/updated user
     */
    server.resource["^/addUser"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                              shared_ptr<HttpServer::Request> request) {
        handleUserRequest(request, response);
    };

    //GET-example for the path /info
    //Responds with request-information
    server.resource["^/users"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                           shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            printAllUsers(response);
        });
        work_thread.detach();
    };

    //GET-example for the path /match/[number], responds with the matched string in path (number)
    //For instance a request GET /match/123 will receive: 123
    server.resource["^/user/([0-9]+)$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                           shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            printUserWithId(request, response);
        });
        work_thread.detach();
    };

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
    server.resource["^/active"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                             shared_ptr<HttpServer::Request> request) {
        handleUserActivity(request, response);
    };

    server.resource["^/getActive"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                               shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            getActiveUsers(request, response);
        });
        work_thread.detach();
    };
    server.resource["^/getRecentUsers"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                                    shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            getRecentUsers(request, response);
        });
        work_thread.detach();
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
    // respond to a list of user id, with the details of all users
    // request must have user id, secret key
    server.resource["^/users_info"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                 shared_ptr<HttpServer::Request> request) {
        thread work_thread([request, response] {
            getUsersDetails(request, response);
        });
        work_thread.detach();
    };
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

void FCMServer::handleUserRequest(Request request,
                                  Response response) {
    try {
        string content = request->content.string();
        User user = UserBuilder::buildFromJson(content);
        std::string esit;
        json respose_json;

        if (user.id == NEW_USER_DEFAULT_ID) { // new user, send secret key
            esit = "############################################### New user created";
            respose_json[USER_SECRET_KEY] = user.secretKey;
        } else
            esit = "############################################### User updated";
        syslog(LOG_INFO, esit.c_str());

        long newId = UserPreferenceProvider().updateOrInsertUser(user);
        respose_json[USER_ID] = newId;
        std::string repose_str = respose_json.dump();

        *response << "HTTP/1.1 200 OK\r\n"
                  << "Content-Type: application/json\r\n"
                  << "Content-Length: " << repose_str.length() << "\r\n\r\n"
                  << repose_str;
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        FCMServer::outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::printAllUsers(Response response) {
    try {
        static WebCacher userWebCacher(generateAllUsers, 5000);
        outputHttpOKStringResponse(userWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
    syslog(LOG_INFO, "AllUsers printed!");
}

void FCMServer::printUserWithId(Request request, Response response) {
    try {
        int user_id = std::stol(request->path_match[1]);

        User user = UserPreferenceProvider::getUser(user_id);
        json jsonObj = UserBuilder::userToJson(user);

        outputHttpOKStringResponse(jsonObj.dump(3), response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::printReportsInInterval(Request request, Response response) {
    try {
        //todo fare questa cosa in modo più efficiente
        long from_millis = std::stol(request->path_match[1]);
        long to_millis = std::stol(request->path_match[2]);
        //stringstream ss;
        //ss << "from: " << from_millis << " to:" << to_millis;
        //syslog(LOG_INFO, ss.str().c_str());
        string res = getReportsFromToMillis(from_millis, to_millis);

        outputHttpOKStringResponse(res, response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

string FCMServer::getReportsRelatedToEvent(Event &e) {
    vector<DBReport> res = ReportProvider::getReportsRelatedToEvents(e);
    return ReportBuilder::reportsToJson(res).dump(3);
}

string FCMServer::getReportsFromToMillis(long from_millis, long to_millis) {
    vector<DBReport> res = ReportProvider::getReportsFromToTime(from_millis, to_millis);
    return ReportBuilder::reportsToJson(res).dump(3);
}

void FCMServer::printEventRelatedReports(Request request, Response response) {
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

        outputHttpOKStringResponse(res, response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::handleReport(FCMServer::Request request, FCMServer::Response response) {
    try {
        static SimpleEQDetector detector;
        string content = request->content.string();
        Report r = ReportParserHTTP::parseRequest(content);

        syslog(LOG_INFO, "New report received!");

        json json_resp;
        json_resp["respose"] = "Report send!";
        outputHttpOKStringResponse(json_resp.dump(3), response);

        thread work_thread([response, r] {
            detector.addReport(r);
        });
        work_thread.detach();
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        outputHttpBADStringResponse(resp, response);

    }
}

void FCMServer::handleUserActivity(FCMServer::Request request, FCMServer::Response response) {
    try {
        string content = request->content.string();
        long id = ReportParserHTTP::parseActiveRequest(content);
        User user = UserPreferenceProvider::getUser(id);

        syslog(LOG_INFO, "User %ld (%s) report to be active!", id, user.username.c_str());

        json json_resp;
        json_resp["respose"] = "Active notify succeded!";
        string message = json_resp.dump(3);
        outputHttpOKStringResponse(message, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::getActiveUsers(FCMServer::Request /*request*/, FCMServer::Response response) {
    try {
        static WebCacher activeUserWebCacher(FCMServer::generateActiveUsers, 5000);
        outputHttpOKStringResponse(activeUserWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);

    }
}

void FCMServer::getRecentUsers(FCMServer::Request /*request*/, FCMServer::Response response) {
    try {
        static WebCacher recentUserWebCacher(FCMServer::generateRecentUsers, 5000);
        outputHttpOKStringResponse(recentUserWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::printDetectedEvents(FCMServer::Request /*request*/, FCMServer::Response response) {
    try {
        static WebCacher detectedEventCacher(FCMServer::generateDetectedEvents,
                                             5000);
        outputHttpOKStringResponse(detectedEventCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::printDetectedEventsFromToMillis(FCMServer::Request request, FCMServer::Response response) {
    try {
        long from = std::stol(request->path_match[1]);
        long to = std::stol(request->path_match[2]);

        string res = FCMServer::generateDetectedEventsFromToMillis(from, to);
        outputHttpOKStringResponse(res, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        outputHttpBADStringResponse(resp, response);
    }
}

void FCMServer::getUsersDetails(FCMServer::Request request, FCMServer::Response response) {
    try {
        string content = request->content.string();
        vector<long> ids = UserBuilder::getUserIdList(content);

        json json_array;
        for (long id : ids) {
            User u = UserPreferenceProvider::getUser(id);
            if (u.hasId())
                json_array.push_back(UserBuilder::userToJson(u));
        }

        json json_resp;
        json_resp["response"] = "successfully";
        json_resp["users"] = json_array;
        string message = json_resp.dump(3);

        outputHttpOKStringResponse(message, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        outputHttpBADStringResponse(resp, response);

    }
}