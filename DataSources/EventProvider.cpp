#include <iostream>
#include "EventProvider.hpp"
#include "INGVDataSource.hpp"


std::vector<Event>  EventProvider::requestEventWebUpdate(){
	using namespace std;

    vector<unique_ptr<DataSource>> sources;
    sources.push_back(unique_ptr<DataSource>(new INGVDataSource));
	vector<Event> results;

	for (unique_ptr<DataSource> & thisDatasource: sources){
		vector<Event> thisResults = thisDatasource->requestEvents();
		results.insert(results.end(), thisResults.begin(), thisResults.end());
	}

	std::clog<<"numero di eventi presi: "<<results.size()<<"\n";
	return results;
}
