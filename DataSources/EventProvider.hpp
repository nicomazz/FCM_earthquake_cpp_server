#ifndef EVENT_DATA_SOURCE_HPP
#define	EVENT_DATA_SOURCE_HPP


#include <vector>
#include <memory>
#include "Models/Event.hpp"

class EventProvider{
public:
	EventProvider(){}
	std::vector<Event>  requestEventWebUpdate();

	std::vector<Event> requestEventFromDB();
    void persistEvent(Event e);
    Event getEvent(long id);
    bool isEventPresent(long id);

};

#endif	/* EVENT_DATA_SOURCE_HPP */
