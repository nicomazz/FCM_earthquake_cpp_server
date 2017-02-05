//
// Created by nicomazz97 on 19/12/16.
//

#ifndef SIMPLE_WEB_SERVER_SERVERFUNCTIONS_HPP
#define SIMPLE_WEB_SERVER_SERVERFUNCTIONS_HPP


#include "server_http.hpp"


namespace ServerUtils {
//generation utils
    typedef shared_ptr<SimpleWeb::ServerBase<SimpleWeb::HTTP>::Response> Response;


//output utils
    void outputHttpOKStringResponse(std::string s, Response response, std::string headers = "");

    void outputHttpBADStringResponse(std::string s, Response response);

}

#endif //SIMPLE_WEB_SERVER_SERVERFUNCTIONS_HPP
