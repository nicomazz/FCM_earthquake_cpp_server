//
// Created by nicomazz97 on 19/11/16.
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::unique_ptr
#include <cstdlib>  // std::exit
#include <iostream>
#include <syslog.h>
#include <odb/database.hxx>


#include <odb/connection.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx>

#include "../Models/Event/Event-odb.hpp"

#define DATABASE_NAME "database.sqlite"

class Database {
public:
    static Database &getInstance() {
        static Database instance;
        return instance;
    }

    std::shared_ptr<odb::database>
    getDatabase() {
        if (db)
            return db;
        syslog(LOG_INFO, "first of all");

        odb::database *bad = new odb::sqlite::database(DATABASE_NAME, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
        if (!bad) {
            syslog(LOG_INFO, "database nullo!!");
        } else {
            syslog(LOG_INFO, "database loaded, id: %d", db->id());
        }
        db.reset(bad);
        syslog(LOG_INFO, "before exist?");

        if (!databaseExist()) {
            syslog(LOG_INFO, "creo nuovo database!");
            createDatabase();
            syslog(LOG_INFO, "creato");

        }
        return db;
    }


private:
    std::shared_ptr<odb::database> db;

    bool databaseExist() {
        odb::transaction t(db->begin());
        try {
            db->query<Event>(false);
        }
        catch (const odb::exception &e) {
            return false;
        }
        t.commit();
        return true;
    }

    void createDatabase() {
        using namespace std;
        using namespace odb::core;

        connection_ptr c(db->connection());
        c->execute("PRAGMA foreign_keys=OFF");
        transaction t(c->begin());
        schema_catalog::create_schema(*db);
        t.commit();
        c->execute("PRAGMA foreign_keys=ON");

    }
};

#endif // DATABASE_HXX
