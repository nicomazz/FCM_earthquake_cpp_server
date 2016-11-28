//
// Created by nicomazz97 on 28/11/16.
//


#include "Database.hpp"


Database &Database::getInstance() {
    static Database instance;
    return instance;
}

std::shared_ptr <odb::database>
Database::getDatabase() {
    if (db)
        return db;
    syslog(LOG_INFO, "first of all");

    db.reset(new odb::sqlite::database(DATABASE_NAME, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
    syslog(LOG_INFO, "before exist?");

    if (!databaseExist()) {
        syslog(LOG_INFO, "creo nuovo database!");
        createDatabase();
        syslog(LOG_INFO, "creato");

    }
    return db;
}

bool Database::databaseExist() {
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

void Database::createDatabase() {
    using namespace std;
    using namespace odb::core;

    connection_ptr c(db->connection());
    c->execute("PRAGMA foreign_keys=OFF");
    transaction t(c->begin());
    schema_catalog::create_schema(*db);
    t.commit();
    c->execute("PRAGMA foreign_keys=ON");

}
