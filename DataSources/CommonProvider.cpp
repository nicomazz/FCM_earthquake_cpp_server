//
// Created by nicomazz97 on 29/11/16.
//

#include "CommonProvider.h"
//TODO CONTINUARE QUESTA COSA..
/*
std::vector <T> CommonProvider::get(){
    using namespace odb::core;
    std::vector<Event> results;

    std::shared_ptr<database> db = Database::getInstance().getDatabase();

    {
        typedef odb::result<T> result;

        // session s;
        transaction t(db->begin());

        result r(db->query<T>());

        for (const T &e: r)
            results.push_back(e);

        t.commit();
    }

    return results;
}

long CommonProvider::persist(T &e, bool checkAlreadyPresent = false){
    using namespace odb::core;

    if (checkAlreadyPresent && isPresent(e.id)) // esiste già
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

T get(long id);

void erase(long id);

bool isPresent(long id);
*/