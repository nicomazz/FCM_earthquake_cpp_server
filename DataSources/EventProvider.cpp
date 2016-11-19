#include <iostream>
#include <sqlite3.h>
#include "EventProvider.hpp"
#include "INGVDataSource.hpp"
#include "../Models/Event-odb.hpp"

#include "../Models/database.hpp"


std::vector<Event> EventProvider::requestEventWebUpdate() {
    using namespace std;

    vector<unique_ptr<DataSource>> sources;
    sources.push_back(unique_ptr<DataSource>(new INGVDataSource));
    vector<Event> results;

    for (unique_ptr<DataSource> &thisDatasource: sources) {
        vector<Event> thisResults = thisDatasource->requestEvents();
        results.insert(results.end(), thisResults.begin(), thisResults.end());
    }

    std::clog << "numero di eventi presi: " << results.size() << "\n";

    return results;
}

std::vector<Event> EventProvider::requestNewEventNotInDB(){
    std::vector<Event> all_events = requestEventWebUpdate();
    std::vector<Event> new_events;
    for (Event & e : all_events){
        if (isEventPresent(e.id))continue;
        new_events.push_back(e);
    }
    return new_events;
}

std::vector<Event> EventProvider::requestEventFromDB() {
    using namespace odb::core;
    std::vector<Event> results;

    std::unique_ptr<database> db(getDB());

    {
        typedef odb::query<Event> query;
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

void EventProvider::persistEvent(Event e, bool checkAlreadyPresent) {
    using namespace odb::core;

    if (checkAlreadyPresent && isEventPresent(e.id)) // esiste già
        return;

    try {
        std::unique_ptr<database> db(getDB());
        {
            transaction t(db->begin());
            db->persist(e);
            t.commit();
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

Event EventProvider::getEvent(long id) {
    using namespace odb::core;
    typedef odb::query<Event> query;
    typedef odb::result<Event> result;

    try {
        std::unique_ptr<database> db(getDB());
        transaction t(db->begin());

        result r(db->query<Event>(query::id == id));
       // result<Event> r(db->query<Event>());


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

