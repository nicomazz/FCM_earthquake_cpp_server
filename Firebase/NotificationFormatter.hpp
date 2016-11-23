//todo NotificationFormatter
// da un evento crea il testo per la notifica da inviare tramite firebase

class NotificationDataBuilder{
public:
	static std::string getJsonForEvent(Event e){
		  //todo move this in notification formatter!
    json j;
    json data;
    data["details"] = getEventDetailsToSend(event);
    j["data"] = data;
    j["to"] = user.firebaseID;
    std::cout<<"dump json: "<<j.dump(3)<<"\n";
    return j.dump();
	}
}