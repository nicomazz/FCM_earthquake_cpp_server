#include "Event.hpp"

Event::Event(int id, std::string location){
	this->id = id;
	this->eventLocation = location;
}


/**
todo list:
- parse eventi da ingv
- connessione con un qualunque tipo di db
- gestire id persone con preferenze
- inviare richiesta di notifica
*/

