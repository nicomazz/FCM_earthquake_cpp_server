//
// Created by nicomazz97 on 10/12/16.
//

#include "../Models/Report/DBReport-odb.hpp"
#include "ReportProvider.hpp"
#include "Database.hpp"

long ReportProvider::persistReport(const Report &r) {
    using namespace odb::core;

    DBReport dbReport;
    dbReport.user_id = r.u.id;
    dbReport.millis = r.millis;
    dbReport.power = r.power;
    dbReport.lat = r.u.lat;
    dbReport.lng = r.u.lng;
    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        {
            transaction t(db->begin());
            long inserted_id = db->persist(dbReport);
            t.commit();
            return inserted_id;
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return -1;
}
std::vector<DBReport> ReportProvider::getReportsFromToTime(long from_millis, long to_millis){
    using namespace odb::core;
    std::vector<DBReport> results;

    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        {
            // session s;
            transaction t(db->begin());

            result<DBReport> r(db->query<DBReport>(query<DBReport>::millis >= from_millis && query<DBReport>::millis <= to_millis));

            for (const DBReport &e: r)
                results.push_back(e);

            t.commit();
        }
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return results;
}

void ReportProvider::deleteReport( Report & r) {
    using namespace odb::core;

    DBReport dbReport;
    dbReport.millis = r.millis;
    try {
        std::shared_ptr<database> db = Database::getInstance().getDatabase();
        transaction t(db->begin());
        db->erase(dbReport);
        t.commit();
    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}