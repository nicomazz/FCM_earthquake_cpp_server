//
// Created by nicomazz97 on 01/12/16.
//

#include <vector>
#include <Utility/GeoUtility.hpp>
#include <iostream>
#include "SimpleEQDetector.hpp"
#include "Report.hpp"




long SimpleEQDetector::millisLastNotifySend = 0;

void SimpleEQDetector::addReport(const Report &report) {
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        reports.insert(report);
    }
    removeOldReports();
    elaborateActualReports();
}

void SimpleEQDetector::addReports(const std::vector<Report> rs) {
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        for (Report r: rs)
            reports.insert(r);
    }
    removeOldReports();
    elaborateActualReports();
}

void SimpleEQDetector::elaborateActualReports() {
    std::lock_guard<std::mutex> guard(v_mutex);
    for (const Report &actual : reports) {
        int cnt_near = 0;
        for (const Report &other: reports) {
            if (actual == other) continue;
            if (getDistance(actual, other) < NEAR_METERS)
                cnt_near++;
        }
        if (cnt_near >= MIN_NEAR_REPORTS) {
            sendNotification(actual);
            removeNear(actual);
            return;
        }
    }
}
void SimpleEQDetector::removeNear(const Report & r){
    std::vector<Report> to_remove;
    for (const Report &other: reports) {
        if (getDistance(r, other) < NEAR_METERS)
            to_remove.push_back(other);
    }
    for (Report & to_rem: to_remove)
        reports.erase(to_rem);
}
double SimpleEQDetector::getDistance(const Report &a, const Report &b) {
    return GeoUtility::distanceEarth(a.u.lat, a.u.lng, b.u.lat, b.u.lng);
}

void SimpleEQDetector::removeOldReports() {

    std::lock_guard<std::mutex> guard(v_mutex);

    while (!reports.empty() && isToRemove(*(reports.begin())) )
        reports.erase(reports.begin());
}


//todo tenere conto anche dell'intensità
bool SimpleEQDetector::isToRemove(const Report &r) {
    return TimeUtils::getCurrentMillis() - r.millis > REPORT_TTL;
}

void SimpleEQDetector::sendNotification(const Report& /*&r*/) {
    millisLastNotifySend = TimeUtils::getCurrentMillis();
    //TODO
}

int SimpleEQDetector::size() {
    std::lock_guard<std::mutex> guard(v_mutex);
    return reports.size();
}

void SimpleEQDetector::clear() {
    std::lock_guard<std::mutex> guard(v_mutex);
    reports.clear();

}


