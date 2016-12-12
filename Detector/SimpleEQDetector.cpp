//
// Created by nicomazz97 on 01/12/16.
//

#include <vector>
#include <Utility/GeoUtility.hpp>
#include <iostream>
#include <Firebase/FirebaseNotificationManager.hpp>
#include <DataSources/EventProvider.hpp>
#include "SimpleEQDetector.hpp"
#include "../DataSources/ReportProvider.hpp"
#include "Report.hpp"


long SimpleEQDetector::millisLastNotifySend = 0;

std::string SimpleEQDetector::getDetectorName() {
    return "SimpleEQDetector";
}

void SimpleEQDetector::addReport(const Report &report) {
    removeUserReport(report.u);
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        reports.insert(report);
        user_last_millis[report.u.id] = report.millis; // update user last report
    }
    ReportProvider::persistReport(report);
    removeOldReports();
    elaborateActualReports();
}

void SimpleEQDetector::removeUserReport(const User &u) {
    std::lock_guard<std::mutex> guard(v_mutex);
    long last_millis = user_last_millis[u.id];
    if (last_millis != 0) {//remove precedent report
        Report to_remove(last_millis);
        reports.erase(to_remove);
    }
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
    float max_percent = 0.0;
    for (const Report &actual : reports) {
        int nearActiveUsers = getNearActiveUsers(actual.u);
        //must be al least 3
        if (nearActiveUsers < MIN_USER_DETECTED_EQ) continue;

        std::vector<Report> near = getNearReports(actual);
       // std::cout<<"tpercent: "<<near.size()<<"/"<<nearActiveUsers<<"\n";
        float percent = (float) near.size() / nearActiveUsers;
        max_percent = std::max(max_percent, percent);
        syslog(LOG_INFO, "att percent: %f", percent);
       // std::cout<<"att percent: "<<percent<<"\n";
        if (percent > MIN_PERCENT){
            sendNotification(near);
            removeNear(actual);
            return;
        }
    }
    syslog(LOG_INFO, "Found at max %f people, not %f", max_percent, MIN_PERCENT);
}

std::vector<Report> SimpleEQDetector::getNearReports(const Report &r) {
    std::vector<Report> near_report;
    for (const Report &other : reports) {
        if (getDistance(r, other) < NEAR_METERS)
            near_report.push_back(other);
    }
    return near_report;
}

void SimpleEQDetector::removeNear(const Report &r) {
    std::vector<Report> to_remove = getNearReports(r);
    for (Report &to_rem : to_remove)
        reports.erase(to_rem);
}


void SimpleEQDetector::sendNotification(const std::vector<Report> &reports) {
    syslog(LOG_INFO, "Sending notification for detected earthquake!");
    millisLastNotifySend = TimeUtils::getCurrentMillis();

    Event e = generateEventFromReports(reports);
    EventProvider::persistEvent(e);
    lastNotified = e;

    //send notification:
    FirebaseNotificationManager notificationManager;
    notificationManager.handleEventNotification(e);
}

int SimpleEQDetector::size() {
    std::lock_guard<std::mutex> guard(v_mutex);
    return (int) reports.size();
}

void SimpleEQDetector::clear() {
    std::lock_guard<std::mutex> guard(v_mutex);
    reports.clear();
}


void SimpleEQDetector::addReports(const std::vector<Report> rs) {
    {
        std::lock_guard<std::mutex> guard(v_mutex);
        for (Report r : rs)
            reports.insert(r);
    }
    removeOldReports();
    elaborateActualReports();
}

Event SimpleEQDetector::getLastEventNotified() {
    return lastNotified;
}

int SimpleEQDetector::getNearActiveUsers(const User &target) {
    std::vector<User> active = UserPreferenceProvider::requestActiveUsers();
    int cnt = 0;
    for (User & u: active)
        if (getDistance(u,target) < NEAR_METERS)
            cnt++;
    return cnt;
}


