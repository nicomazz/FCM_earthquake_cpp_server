//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_USERBUILDER_H
#define SIMPLE_WEB_SERVER_USERBUILDER_H


#include <stdexcept>
#include "../../Utility/json.hpp"
#include <syslog.h>
#include "User-odb.hpp"
#include "user_field.hpp"
#include <random>

using json = nlohmann::json;
using namespace std;

#define SECRET_KEY_LENGTH 42
#define NEW_USER_DEFAULT_ID -1

class UserBuilder {

public:
    /**
     * crea un utente da una stringa json. Nel caso ci siano problemi di parse
     * o di attributi mancanti viene lanciata un'eccezione
     * @param json_string
     * @return
     */
    static User buildFromJson(std::string json_string);

    static json userToJson(User &u);

    static std::string generateRandomString(int l = SECRET_KEY_LENGTH);

    /**
     * id:
     * secretKey:
     * ids: []
     * */
    static std::vector<long> getUserIdList(string json_array);

    template<typename T>
    static T get(json j, std::string key);

    static void addDBFields(User& user);
};


#endif //SIMPLE_WEB_SERVER_USERBUILDER_H
