//
// Created by nicomazz97 on 26/11/16.
//

#include <Firebase/NotificationDataBuilder.hpp>
#include <Detector/ReportParserHTTP.hpp>
#include "ServerInitializer.hpp"

void FCMServer::handleUserRequest(shared_ptr<HttpServer::Request> request,
                                  shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> response) {
    try {
        string content = request->content.string();
        User user = UserBuilder::buildFromJson(content);
        std::string esit;
        json respose_json;

        if (user.id == NEW_USER_DEFAULT_ID) { // new user, send secret key
            esit = "################################################################# New user created";
            respose_json[USER_SECRET_KEY] = user.secretKey;
        } else
            esit = "################################################################# User updated";
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
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << resp.length() << "\r\n\r\n"
                  << resp;
    }
}

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
    server.resource["^/info$"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                           shared_ptr<HttpServer::Request> request) {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " ("
                       << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for (auto &header: request->header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }

        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);

        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n"
                  << content_stream.rdbuf();
    };
    //GET-example for the path /info
    //Responds with request-information
    server.resource["^/users"]["GET"] = [](shared_ptr<HttpServer::Response> response,
                                           shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            try {
                stringstream content_stream;
                UserPreferenceProvider userProvider;
                std::vector<User> allUsers = userProvider.requestUsersFromDB();
                json jsonObj;
                for (User &u : allUsers)
                    jsonObj.push_back(UserBuilder::userToJson(u));

                content_stream << jsonObj.dump(3);

                //find length of content_stream (length received using content_stream.tellp())
                content_stream.seekp(0, ios::end);

                *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n"
                          << content_stream.rdbuf();
            } catch (exception &e) {
                string resp(e.what());
                syslog(LOG_INFO, e.what());
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << resp.length() << "\r\n\r\n"
                          << resp;
            }
        });
        work_thread.detach();
    };
    //GET-example for the path /match/[number], responds with the matched string in path (number)
    //For instance a request GET /match/123 will receive: 123
    server.resource["^/user/([0-9]+)$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                           shared_ptr<HttpServer::Request> request) {
        thread work_thread([response, request] {
            try {
                int user_id = std::stoi(request->path_match[1]);

                stringstream content_stream;
                UserPreferenceProvider userProvider;
                User user = userProvider.getUser(user_id);
                json jsonObj = UserBuilder::userToJson(user);

                content_stream << jsonObj.dump(3);

                //find length of content_stream (length received using content_stream.tellp())
                content_stream.seekp(0, ios::end);

                *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n"
                          << content_stream.rdbuf();
            }
            catch (exception &e) {
                string resp(e.what());
                syslog(LOG_INFO, e.what());
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << resp.length() << "\r\n\r\n"
                          << resp;
            }
        });
        work_thread.detach();
    };

    server.resource["^/report"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                             shared_ptr<HttpServer::Request> request) {
        try {
            static SimpleEQDetector detector;
            string content = request->content.string();
            Report r = ReportParserHTTP::parseRequest(content);

            json json_resp;
            json_resp["respose"] = "Report send!";
            string message = json_resp.dump(3);
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;

            thread work_thread([response, r] {
                detector.addReport(r);
            });
            work_thread.detach();
        } catch (exception &e) {
            string resp(e.what());
            syslog(LOG_INFO, e.what());
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << resp.length() << "\r\n\r\n"
                      << resp;
        }
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

