#ifndef EVENT_DATA_SOURCE_HPP
#define    EVENT_DATA_SOURCE_HPP


#include <vector>
#include <memory>
#include "Models/Event/Event.hpp"
#include <iostream>
#include <sqlite3.h>
#include "DataSources/EventsWebProvider/INGVDataSource.hpp"
#include "Models/Event/Event-odb.hpp"

#include "Database.hpp"

class EventProvider {
public:
    EventProvider() {}

    static std::vector<Event> requestEventWebUpdate();

    static std::vector<Event> requestNewEventNotInDB();

    static std::vector<Event> requestEventFromDB();

    static std::vector<Event> requestDetectorEvents();

    static long persistEvent(Event &e, bool checkAlreadyPresent = false);

    static Event getEvent(long id);

    static void deleteEvent(Event e);

    static bool isEventPresent(long id);

};

#endif	/* EVENT_DATA_SOURCE_HPP */
