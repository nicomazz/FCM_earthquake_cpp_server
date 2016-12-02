//
// Created by nicomazz97 on 01/12/16.
//

#include <vector>
#include <Utility/GeoUtility.hpp>
#include "SimpleEQDetector.hpp"
#include "Report.hpp"


#define  REPORT_TTL (1000*60*3)
#define  NEAR_METERS 20 * 1000
#define  MIN_NEAR_REPORTS 3

void SimpleEQDetector::addReports(Report &report) {
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        reports.insert(report);
    }
    removeOldReports();
    elaborateActualReports();
}

void SimpleEQDetector::elaborateActualReports() {
    std::lock_guard<std::mutex> guard(v_mutex);
    for (const Report & actual : reports){
        int cnt_near = 0;
        for (const Report & other: reports){
            if (actual == other) continue;
            if (getDistance(actual,other) < NEAR_METERS )
                cnt_near++;
        }
        if (cnt_near >= MIN_NEAR_REPORTS) {
            sendNotification(actual);
            reports.clear();
            return;
        }
    }
}

double SimpleEQDetector::getDistance(const Report & a, const Report & b){
    GeoUtility::distanceEarth(a.u.lat,a.u.lng,b.u.lat,b.u.lng);

}

void SimpleEQDetector::removeOldReports() {

    std::lock_guard<std::mutex> guard(v_mutex);

    // rimoviamo i più vecchi di una certa soglia
    long from_to_remove = TimeUtils::getCurrentMillis() - REPORT_TTL;
    Report dummyReport(from_to_remove);
    std::set<Report>::iterator it_from_remove = reports.lower_bound(dummyReport);
    while (it_from_remove != reports.end())
        reports.erase(it_from_remove++);
}


//todo tenere conto anche dell'intensità
bool SimpleEQDetector::isToRemove(const Report &r) {
    return TimeUtils::getCurrentMillis() - r.millis > REPORT_TTL;
}

void SimpleEQDetector::sendNotification(const Report &r) {
    //TODO
}
