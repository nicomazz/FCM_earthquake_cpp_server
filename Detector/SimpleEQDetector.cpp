//
// Created by nicomazz97 on 01/12/16.
//

#include <vector>
#include <Utility/GeoUtility.hpp>
#include <iostream>
#include <Firebase/FirebaseNotificationManager.hpp>
#include <DataSources/EventProvider.hpp>
#include "SimpleEQDetector.hpp"
#include "Report.hpp"


long SimpleEQDetector::millisLastNotifySend = 0;

std::string SimpleEQDetector::getDetectorName() {
    return "SimpleEQDetector";
}

void SimpleEQDetector::addReport(const Report &report) {
    removeAndUpdateUserReport(report);
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        reports.insert(report);
    }
    //todo use conditional variable
    removeOldReports();
    elaborateActualReports();
}

void SimpleEQDetector::removeAndUpdateUserReport(const Report &report){
    
      std::lock_guard<std::mutex> guard(v_mutex);
      long last_millis = user_last_millis[report.u.id];
      if (last_millis != 0) {//remove precedent report
          Report to_remove(last_millis);
          reports.erase(to_remove);
      }
    user_last_millis[id] = report.millis;
   
}
void SimpleEQDetector::removeOldReports() {
    std::lock_guard<std::mutex> guard(v_mutex);
    while (!reports.empty() && isToRemove(*(reports.begin())))
        reports.erase(reports.begin());
}

//todo tenere conto anche dell'intensità
bool SimpleEQDetector::isToRemove(const Report &r) {
    return TimeUtils::getCurrentMillis() - r.millis > REPORT_TTL;
}

void SimpleEQDetector::elaborateActualReports() {
    std::lock_guard<std::mutex> guard(v_mutex);
    int max_near = 0;
    for (const Report &actual : reports) {
        std::vector<Report> near = getNearReports(actual);
        max_near = std::max(max_near, (int) near.size());
        if (near.size() >= MIN_NEAR_REPORTS) {
            sendNotification(actual);
            removeNear(actual);
            return;
        }
    }
    syslog(LOG_INFO, "Found at max %d people, not %d",max_near,MIN_NEAR_REPORTS);
}

std::vector<Report> SimpleEQDetector::getNearReports(const Report &r) {
    std::vector<Report> near_report;
    for (const Report &other: reports) {
        if (r == other) continue;
        if (getDistance(r, other) < NEAR_METERS)
            near_report.push_back(other);
    }
    return near_report;
}

void SimpleEQDetector::removeNear(const Report &r) {
    std::vector<Report> to_remove = getNearReports(r);
    for (Report &to_rem: to_remove)
        reports.erase(to_rem);
}



void SimpleEQDetector::sendNotification(const Report &r) {
    syslog(LOG_INFO, "Sending notification for detected earthquake!");
    millisLastNotifySend = TimeUtils::getCurrentMillis();

    Event e = generateEventFromReport(r);
    EventProvider::persistEvent(e);

    //send notification:
    FirebaseNotificationManager notificationManager;
    notificationManager.handleEventNotification(e);
}

int SimpleEQDetector::size() {
    std::lock_guard<std::mutex> guard(v_mutex);
    return reports.size();
}

void SimpleEQDetector::clear() {
    std::lock_guard<std::mutex> guard(v_mutex);
    reports.clear();
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


