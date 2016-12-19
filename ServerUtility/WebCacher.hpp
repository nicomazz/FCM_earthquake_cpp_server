//
// Created by nicomazz97 on 19/12/16.
//

#ifndef SIMPLE_WEB_SERVER_WEB_CACHER_H
#define SIMPLE_WEB_SERVER_WEB_CACHER_H
#include <string>

#define WEB_CACHER_TTL_DEFAULT 5000
/**
 * due to the high numer of request we cache the respose 
 * of each requests to strings with max ttl. Each time
 * the ttl expire we renew the response string with a 
 * std::function<std::string()>
 */
class WebCacher {
public:
    WebCacher(std::function<std::string()> & generator, 
        int ttl_millis = WEB_CACHER_TTL_DEFAULT);

    std::string getResponse();
private:
    std::string response;
    int ttl;
    int lastUpdate;
    std::mutex response_mutex;
    std::function<std::string()> generator;
};

#endif //SIMPLE_WEB_SERVER_WEB_CACHER_H
