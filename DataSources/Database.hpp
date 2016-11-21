//
// Created by nicomazz97 on 19/11/16.
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::unique_ptr
#include <cstdlib>  // std::exit
#include <iostream>

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

        db.reset(new odb::sqlite::database(DATABASE_NAME, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE));

        if (!databaseExist())
            createDatabase();
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
