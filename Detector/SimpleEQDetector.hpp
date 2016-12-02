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

/**
 * based on a very simple and stupid idea, with many problem:
 * if there are al least N person who dist each other less than K KM
 * that has sent a report we can say that an earthquake has been detected
 */
class SimpleEQDetector : public GenericEQDetector {
public:
    virtual void addReports(const Report &r) override;

private:
    std::set<Report> reports;

    std::mutex v_mutex;

    virtual void elaborateActualReports() override;

    void removeOldReports();

    bool isToRemove(const Report &r);

    double getDistance(const Report &a, const Report &b);

    void sendNotification(const Report &r);

};


#endif //SIMPLE_WEB_SERVER_SIMPLEEQDETECTOR_H
