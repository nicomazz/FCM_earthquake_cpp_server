#ifndef EVENT_DATA_SOURCE_HPP
#define	EVENT_DATA_SOURCE_HPP


#include <vector>
#include "Models/Event.hpp"

class EventProvider{
public:
	EventProvider(){}
	std::vector<Event>  requestEventWebUpdate();

};

#endif	/* EVENT_DATA_SOURCE_HPP */
