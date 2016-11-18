#include <iostream>
#include "EventProvider.hpp"
#include "INGVDataSource.hpp"


std::vector<Event>  EventProvider::requestEventWebUpdate(){
	using namespace std;

	DataSource sources[] = {INGVDataSource()}; // add here other dataSource
	int sz = sizeof(sources)/sizeof(sources[0]);
	vector<Event> results;

	for (int i = 0; i < sz; i++){
		vector<Event> thisResults = sources[i].requestEvents();
		results.insert(results.end(), thisResults.begin(), thisResults.end());
	}

	std::clog<<"numero di eventi presi: "<<results.size()<<"\n";
	return results;
}
