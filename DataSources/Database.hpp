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

#define USER_DATABASE "UsersPreferences.db"
#define EVENT_DATABASE "Events.db"

class Database {
public:
    static Database &getInstance() {
        static Database instance;
        return instance;
    }

    odb::database *
    getEventDatabase() {
        if (eventDB != nullptr)
            return eventDB;

        try {
            odb::database *db =
                    new odb::sqlite::database(EVENT_DATABASE,
                                              SQLITE_OPEN_READWRITE);
            return eventDB = db;
        } catch ( ...){
            // database da creare!
            return eventDB = createEventDatabase();
        }
    }

    odb::database *
    getUserDatabase() {
        if (userDB != nullptr)
            return userDB;

       // createUsersDatabase();
        try {
            odb::database *db =
                    new odb::sqlite::database(USER_DATABASE,
                                              SQLITE_OPEN_READWRITE);

            return userDB = db;
        } catch ( ...){
            // database da creare!
            return eventDB = createUsersDatabase();
        }


    }

    odb::database *
    createEventDatabase() {
        return createDatabase(EVENT_DATABASE);
    }

    odb::database *
    createUsersDatabase() {
        return createDatabase(USER_DATABASE);
    }

private:
    odb::database *eventDB;
    odb::database *userDB;


    odb::database *
    createDatabase(std::string name) {
        using namespace std;
        using namespace odb::core;

        database *db =
                new odb::sqlite::database(name,
                                          SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        // Create the database schema. Due to bugs in SQLite foreign key
        // support for DDL statements, we need to temporarily disable
        // foreign keys.
        //
        {
            connection_ptr c(db->connection());

            c->execute("PRAGMA foreign_keys=OFF");

            transaction t(c->begin());
            schema_catalog::create_schema(*db);
            t.commit();

            c->execute("PRAGMA foreign_keys=ON");
        }
        return db;
    }
};

#endif // DATABASE_HXX
