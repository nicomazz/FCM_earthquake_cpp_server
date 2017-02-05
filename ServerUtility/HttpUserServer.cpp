//
// Created by nicomazz97 on 05/02/17.
//

#include "HttpUserServer.hpp"

void HttpUserServer::initUserFunctions(SimpleWeb::Server<SimpleWeb::HTTP> &server) {
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
    // respond to a list of user id, with the details of all users
    // request must have user id, secret key
    server.resource["^/users_info"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                 shared_ptr<HttpServer::Request> request) {
        thread work_thread([request, response] {
            getUsersDetails(request, response);
        });
        work_thread.detach();
    };
}

void HttpUserServer::handleUserRequest(Request request,
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
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpUserServer::printAllUsers(Response response) {
    try {
        static WebCacher userWebCacher(generateAllUsers, 5000);
        ServerUtils::outputHttpOKStringResponse(userWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
    syslog(LOG_INFO, "AllUsers printed!");
}

void HttpUserServer::printUserWithId(Request request, Response response) {
    try {
        int user_id = std::stol(request->path_match[1]);

        User user = UserPreferenceProvider::getUser(user_id);
        json jsonObj = UserBuilder::userToJson(user);

        ServerUtils::outputHttpOKStringResponse(jsonObj.dump(3), response);
    }
    catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpUserServer::handleUserActivity(Request request, Response response) {
    try {
        string content = request->content.string();
        long id = ReportParserHTTP::parseActiveRequest(content);
        User user = UserPreferenceProvider::getUser(id);

        syslog(LOG_INFO, "User %ld (%s) report to be active!", id, user.username.c_str());

        json json_resp;
        json_resp["respose"] = "Active notify succeded!";
        string message = json_resp.dump(3);
        ServerUtils::outputHttpOKStringResponse(message, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpUserServer::getActiveUsers(Request /*request*/, Response response) {
    try {
        static WebCacher activeUserWebCacher(generateActiveUsers, 5000);
        ServerUtils::outputHttpOKStringResponse(activeUserWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);

    }
}

void HttpUserServer::getRecentUsers(Request /*request*/, Response response) {
    try {
        static WebCacher recentUserWebCacher(generateRecentUsers, 5000);
        ServerUtils::outputHttpOKStringResponse(recentUserWebCacher.getResponse(), response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, e.what());
        ServerUtils::outputHttpBADStringResponse(resp, response);
    }
}

void HttpUserServer::getUsersDetails(Request request, Response response) {
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

        ServerUtils::outputHttpOKStringResponse(message, response);
    } catch (exception &e) {
        string resp(e.what());
        syslog(LOG_INFO, resp.c_str());
        ServerUtils::outputHttpBADStringResponse(resp, response);

    }
}

std::string HttpUserServer::generateAllUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestUsersFromDB();
    json jsonObj;

    for (User &u : allUsers)
        jsonObj.push_back(UserBuilder::userToJson(u));
    return jsonObj.dump(3);
}

std::string HttpUserServer::generateActiveUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestActiveUsers();

    json jsonObj = json::array();

    for (User &u : allUsers)
        if (u.hasPosition())
            jsonObj.push_back(UserBuilder::userToJson(u));

    return jsonObj.dump(3);
}

std::string HttpUserServer::generateRecentUsers() {
    UserPreferenceProvider userProvider;
    std::vector<User> allUsers = userProvider.requestRecentUsers();

    json jsonObj = json::array();

    for (User &u : allUsers) {
        if (u.hasPosition())
            jsonObj.push_back(UserBuilder::userToJson(u));
    }

    return jsonObj.dump(3);
}