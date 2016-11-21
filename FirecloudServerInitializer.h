//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H
#define SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H

#include "server_http.hpp"
#include "client_http.hpp"

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

namespace FCMServer {
    typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
    typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

    using namespace std;
    using namespace boost::property_tree;


    void default_resource_send(const HttpServer &server, const shared_ptr<HttpServer::Response> &response,
                               const shared_ptr<ifstream> &ifs) {
        //read and send 128 KB at a time
        static vector<char> buffer(131072); // Safe when server is running on one thread
        streamsize read_length;
        if ((read_length = ifs->read(&buffer[0], buffer.size()).gcount()) > 0) {
            response->write(&buffer[0], read_length);
            if (read_length == static_cast<streamsize>(buffer.size())) {
                server.send(response, [&server, response, ifs](const boost::system::error_code &ec) {
                    if (!ec)
                        default_resource_send(server, response, ifs);
                    else
                        cerr << "Connection interrupted" << endl;
                });
            }
        }
    }

    void initServer(SimpleWeb::Server<SimpleWeb::HTTP> &server) {

        /**
         * handle request to add/update user info.
         * the respose is a json with the id of the new/updated user
         */
        server.resource["^/addUser"]["POST"] = [](shared_ptr<HttpServer::Response> response,
                                                  shared_ptr<HttpServer::Request> request) {
            try {
                string content = request->content.string();
                User user = UserBuilder::buildFromJson(content);

                std::string esit;
                if (user.id  < 0) // non esisteva!
                    esit = "New user created";
                else
                    esit = "user updated";
                cout<<esit<<"\n";

                long newId = UserPreferenceProvider().handleNewUserRequest(user);
                stringstream ss; ss<<"{ \"id\":"<<newId<<"}";
                std::string resp = ss.str();

                *response << "HTTP/1.1 200 OK\r\n"
                          << "Content-Type: application/json\r\n"
                          << "Content-Length: " << resp.length() << "\r\n\r\n"
                          << resp;
            }
            catch (exception &e) {
                string resp(e.what());
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << resp.length() << "\r\n\r\n"
                          << resp;
            }
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

        //GET-example for the path /match/[number], responds with the matched string in path (number)
        //For instance a request GET /match/123 will receive: 123
        server.resource["^/match/([0-9]+)$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                                shared_ptr<HttpServer::Request> request) {
            string number = request->path_match[1];
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() << "\r\n\r\n" << number;
        };

        //Get example simulating heavy work in a separate thread
        server.resource["^/work$"]["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                      shared_ptr<HttpServer::Request> /*request*/) {
            thread work_thread([response] {
                this_thread::sleep_for(chrono::seconds(5));
                string message = "Work done";
                *response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
            });
            work_thread.detach();
        };

        //Default GET-example. If no other matches, this anonymous function will be called.
        //Will respond with content in the web/-directory, and its subdirectories.
        //Default file: index.html
        //Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
        server.default_resource["GET"] = [&server](shared_ptr<HttpServer::Response> response,
                                                   shared_ptr<HttpServer::Request> request) {
            try {
                auto web_root_path = boost::filesystem::canonical("web");
                auto path = boost::filesystem::canonical(web_root_path / request->path);
                //Check if path is within web_root_path
                if (distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
                    !equal(web_root_path.begin(), web_root_path.end(), path.begin()))
                    throw invalid_argument("path must be within root path");
                if (boost::filesystem::is_directory(path))
                    path /= "index.html";
                if (!(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)))
                    throw invalid_argument("file does not exist");

                auto ifs = make_shared<ifstream>();
                ifs->open(path.string(), ifstream::in | ios::binary);

                if (*ifs) {
                    ifs->seekg(0, ios::end);
                    auto length = ifs->tellg();

                    ifs->seekg(0, ios::beg);

                    *response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
                    default_resource_send(server, response, ifs);
                } else
                    throw invalid_argument("could not read file");
            }
            catch (const exception &e) {
                string content = "Could not open path " + request->path + ": " + e.what();
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n"
                          << content;
            }
        };

    }
}
#endif //SIMPLE_WEB_SERVER_FIRECLOUDSERVERINITIALIZER_H
