//
// Created by nicomazz97 on 01/12/16.
//

#ifndef SIMPLE_WEB_SERVER_SIMPLEEQDETECTOR_H
#define SIMPLE_WEB_SERVER_SIMPLEEQDETECTOR_H

#include "GenericEQDetector.hpp"
#include "Report.hpp"

#include <thread>
#include <mutex>
#include <set>

#define  REPORT_TTL (1000*60*3)
#define  NEAR_METERS 20 * 1000
// at min the MIN_PERCENT users in range of NEAR_METER Must have detected an EQ to send the notify
#define  MIN_PERCENT 0.30f
#define  MIN_NEAR_REPORTS 10
//min number of users that must have detected an EQ to send the notify
#define  MIN_USER_DETECTED_EQ 2
/**
 * based on a very simple and stupid idea, with many problem:
 * if there are al least N person who dist each other less than K KM
 * that has sent a report we can say that an earthquake has been detected
 */
class SimpleEQDetector : public GenericEQDetector {
private:
    std::set<Report> reports;
    std::mutex v_mutex;
    /**
     * map user id in user last millis report.
     * Every user can have only one report in reports.
     * When user make a report first we check if this is another
     * report by that user
     */
    std::map<long,long> user_last_millis;

    Event lastNotified;

public:
    void addReport(const Report &r) override;
    void addReports(const std::vector<Report> rs) override;

    static long millisLastNotifySend;

    int size() override;
    void clear() override;

    std::string getDetectorName() override;

    Event getLastEventNotified();
private:

    virtual void elaborateActualReports() override;

    void removeOldReports();

    std::vector<Report> getNearReports(const Report &r);

    int getNearActiveUsers(const User & u);

    bool isToRemove(const Report &r);

    void sendNotification(const std::vector<Report> &r);

    void removeNear(const Report & r);

    void removeUserReport(const User &u);

};


#endif //SIMPLE_WEB_SERVER_SIMPLEEQDETECTOR_H
