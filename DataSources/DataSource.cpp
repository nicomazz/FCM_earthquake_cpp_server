//
// Created by nicomazz97 on 18/11/16.
//
#include <iostream>
#include "DataSource.hpp"

std::vector<std::string> DataSource::split(std::string &str, char delimiter) {
    static char buffer[5000];
    std::vector<std::string> result;
    int sz = str.size();
    int ind = 0;
    for (int i = 0; i < sz; i++) {

        if (str[i] != delimiter) {
            buffer[ind++] = str[i];
        } else {
            buffer[ind] = 0;
            result.push_back(std::string(buffer));
            ind = 0;
        }
    }
    buffer[ind] = 0;
    result.push_back(std::string(buffer));
    return result;
}

std::vector<Event> DataSource::requestEvents() {
    SimpleWeb::Client<SimpleWeb::HTTP> client(getWebServiceUrl());
    std::string respose;
    {
        std::stringstream output;
        auto r = client.request("GET", getWebServiceUrlParams());
        output << r->content.rdbuf();
        respose = output.str();
    }
    std::clog << "risposta: " << respose;
    // todo check if it is a bad respose
    return parseEvents(respose);
}

