//
// Created by nicomazz97 on 19/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DATABASE_H
#define SIMPLE_WEB_SERVER_DATABASE_H
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

inline std::unique_ptr<odb::database>
getDB ()
{
    using namespace std;
    using namespace odb::core;

#if defined(DATABASE_SQLITE)
    unique_ptr<database> db (
    new odb::sqlite::database ( "Events.db",
    SQLITE_OPEN_READWRITE));
#endif

    return db;
}

inline std::unique_ptr<odb::database>
createDB ()
{
    using namespace std;
    using namespace odb::core;

#if defined(DATABASE_SQLITE)
    unique_ptr<database> db (
            new odb::sqlite::database ( "Events.db",
                                        SQLITE_OPEN_READWRITE));

    // Create the database schema. Due to bugs in SQLite foreign key
    // support for DDL statements, we need to temporarily disable
    // foreign keys.
    //
    {
        connection_ptr c (db->connection ());

        c->execute ("PRAGMA foreign_keys=OFF");

        transaction t (c->begin ());
        schema_catalog::create_schema (*db);
        t.commit ();

        c->execute ("PRAGMA foreign_keys=ON");
    }

#endif

    return db;
}
#endif // DATABASE_HXX
#endif //SIMPLE_WEB_SERVER_DATABASE_H
