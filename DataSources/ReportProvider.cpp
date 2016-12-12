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
