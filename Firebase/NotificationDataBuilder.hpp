//todo NotificationFormatter
// da un evento crea il testo per la notifica da inviare tramite firebase

class NotificationDataBuilder{
public:
	static std::string getJsonForUserEvent(User & user, Event &e);
    static json getJsonDetails(Event e);

}