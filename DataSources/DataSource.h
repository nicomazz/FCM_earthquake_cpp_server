//
// Created by nicomazz97 on 17/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DATASOURCE_H
#define SIMPLE_WEB_SERVER_DATASOURCE_H

#include <Models/Event.hpp>
#include <vector>
#include <sstream>

class DataSource{
public:
    virtual std::vector<Event> requestEvents() = 0;

protected:
    virtual std::string getWebServiceUrl() = 0;
    virtual std::vector<Event> parseEvents(std::string webResponse)  = 0;
    virtual Event buildEvent(std::string s) = 0;

    std::vector <std::string> split(std::string &str, char delimiter) {
        static char buffer[5000];
        std::vector<std::string> result;
        int sz = str.size();
        int ind = 0;
        for (int i = 0; i < sz; i++) {

            if (str[i] != delimiter) {
                buffer[ind++] = str[i];
            }
            else {
                buffer[ind] = 0;
                result.push_back(std::string(buffer));
                ind = 0;
            }
        }
        buffer[ind] = 0;
        result.push_back(std::string(buffer));
        return result;
    }

};
#endif //SIMPLE_WEB_SERVER_DATASOURCE_H
