#include <iostream>
#include "EventProvider.hpp"
#include "INGVDataSource.hpp"


std::vector<Event>  EventProvider::requestEventWebUpdate(){
	//todo richiesta get a web service dei vari data sources..
	// per ora creo 10 eventi e li restituisco
	INGVDataSource ingv;
	std::vector<Event> results = ingv.requestEvents();
	std::clog<<"numero di eventi presi: "<<results.size()<<"\n";
	return results;
}
