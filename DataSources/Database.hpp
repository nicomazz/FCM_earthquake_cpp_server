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
#include "../Models/User/User-odb.hpp"

#define DATABASE_NAME "database.sqlite"

class Database {
public:
    static Database &getInstance();

    std::shared_ptr<odb::database>
    getDatabase();


private:
    std::shared_ptr<odb::database> db;

    bool databaseExist();

    void createDatabase();
};

#endif // DATABASE_HXX
