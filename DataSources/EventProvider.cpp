#include <syslog.h>
#include <DataSources/EventsWebProvider/USGSDataSource.hpp>
#include <Utility/TimeUtility.hpp>
#include <mutex>
#include <Models/Event/EventBuilder.hpp>
#include "EventProvider.hpp"


std::vector<Event> EventProvider::requestEventWebUpdate() {
    return requestEventsFromWebDataSources(TimeUtility::getCurrentMillis());
}

std::vector<Event> EventProvider::requestEventsFromWebDataSources(long fromMillis, long toMillis) {
    using namespace std;

    if (toMillis <= 0) toMillis = fromMillis;
    vector<unique_ptr<WebDataSourceInterface>> sources;
    sources.push_back(unique_ptr<WebDataSourceInterface>(new INGVDataSource));
    sources.push_back(unique_ptr<WebDataSourceInterface>(new USGSDataSource));
    vector<Event> results;

    for (unique_ptr<WebDataSourceInterface> &thisDatasource: sources) {
        vector<Event> thisResults = thisDatasource->requestEventsInDateRange(fromMillis, toMillis);
        results.insert(results.end(), thisResults.begin(), thisResults.end());
    }

    return results;
}

std::vector<Event> EventProvider::requestNewEventNotInDB() {
    std::vector<Event> all_events = EventProvider::requestEventWebUpdate();
    std::vector<Event> new_events;
    for (Event &e : all_events) {
        if (EventProvider::isEventPresent(e.id))continue;
        new_events.push_back(e);
    }
    //syslog(LOG_INFO,EventBuilder::eventsToJson(new_events).dump(3).c_str());
    return new_events;
}

std::vector<Event> EventProvider::requestAndPersistEventFromWebInDateRange(long fromMillis, long toMillis) {
    std::vector<Event> all_events = EventProvider::requestEventsFromWebDataSources(fromMillis, toMillis);
    for (Event &e : all_events) {
        if (EventProvider::isEventPresent(e.id))continue;
        EventProvider::persistEvent(e);
    }
    return all_events;
}


std::vector<Event> EventProvider::requestEventFromDB() {
    using namespace odb::core;
    std::vector<Event> results;

    std::shared_ptr<database> db = Database::getInstance().getDatabase();

    {
        typedef odb::result<Event> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<Event>());

        for (const Event &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}

std::vector<Event> EventProvider::requestEventsInInterval(long from_millis, long to_millis) {
    using namespace odb::core;
    std::vector<Event> results;

    std::shared_ptr<database> db = Database::getInstance().getDatabase();

    {
        typedef odb::result<Event> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<Event>(query<Event>::millis >= from_millis && query<Event>::millis <= to_millis));

        for (const Event &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}

std::vector<Event> EventProvider::requestDetectedEvents() {
    using namespace odb::core;
    std::vector<Event> results;

    std::shared_ptr<database> db = Database::getInstance().getDatabase();

    {
        typedef odb::result<Event> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<Event>(query<Event>::isRealTimeReport == true || query<Event>::numberOfReports > 0));

        for (const Event &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}

std::vector<Event> EventProvider::requestDetectedEvents(long from_millis, long to_millis) {
    using namespace odb::core;
    std::vector<Event> results;

    std::shared_ptr<database> db = Database::getInstance().getDatabase();

    {
        typedef odb::result<Event> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<Event>((query<Event>::numberOfReports > 0 || query<Event>::isRealTimeReport) &&
                                  query<Event>::millis >= from_millis && query<Event>::millis <= to_millis));

        for (const Event &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}


long EventProvider::persistEvent(Event &e, bool checkAlreadyPresent) {
    using namespace odb::core;

    if (checkAlreadyPresent && isEventPresent(e.id)) // esiste già
        return e.id;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        {
            transaction t(db->begin());
            long inserted_id = db->persist(e);
            e.id = inserted_id;
            t.commit();
            return inserted_id;
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return -1;
}

Event EventProvider::getEvent(long id) {
    using namespace odb::core;
    typedef odb::query<Event> query;
    typedef odb::result<Event> result;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());

        result r(db->query<Event>(query::id == id));


        for (const Event e: r) {
            t.commit();
            return e;
        }
        t.commit();

    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return Event();
}

bool EventProvider::isEventPresent(long id) {
    return getEvent(id).id == id;
}

void EventProvider::deleteEvent(Event e) {
    using namespace odb::core;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        db->erase(e);
        t.commit();

    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void EventProvider::removeOldEvents() {
    using namespace odb::core;
    typedef odb::query<Event> query;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        long millis_threshold = TimeUtility::getNDaysAgoMillis(7);
        db->erase_query<Event>(
                query::isRealTimeReport == false && query::numberOfReports == 0 && query::millis<millis_threshold);
        t.commit();
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void EventProvider::updateEvent(Event &e) {
    using namespace odb::core;
    try {
        static std::mutex v_mutex;
        std::lock_guard<std::mutex> guard(v_mutex);
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        db->update(e);
        t.commit();
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

