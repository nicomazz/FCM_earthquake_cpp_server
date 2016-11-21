//
// Created by nicomazz97 on 19/11/16.
//


// file      : c++11/database.hxx
// copyright : not copyrighted - public domain

//
// Create concrete database instance based on the DATABASE_* macros.
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::unique_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <odb/database.hxx>

#define DATABASE_SQLITE

#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx>

#define DATABASE_NAME "database.sqlite"
#define EVENT_DATABASE "Events.db"

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

        db.reset(new odb::sqlite::database(EVENT_DATABASE, SQLITE_OPEN_READWRITE));

        if (!databaseExist())
            createDatabase();
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
