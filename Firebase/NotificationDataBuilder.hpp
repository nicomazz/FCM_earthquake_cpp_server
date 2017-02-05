#ifndef SIMPLE_WEB_SERVER_NOTIFICATOIN_BUILDER_H
#define SIMPLE_WEB_SERVER_NOTIFICATOIN_BUILDER_H

#include <string>
#include "../Models/User/User.hpp"
#include "../Models/Event/Event.hpp"
#include "../Utility/json.hpp"
// da un evento crea il testo per la notifica da inviare tramite firebase
using json = nlohmann::json;

class NotificationDataBuilder {
public:
    static std::string getJsonForUserEvent(User &user, Event &e);

private:
    static json getJsonDetails(Event e);

};

#endif