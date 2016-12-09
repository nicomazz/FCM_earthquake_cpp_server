//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DETECTOR_H
#define SIMPLE_WEB_SERVER_DETECTOR_H

#include <Utility/GeoUtility.hpp>
#include "../Models/User/User.hpp"
#include "Report.hpp"

#define SHAKE_TTL_MS (1000)*(60)*(5)


class GenericEQDetector {
public:
    /**
     * Add a user shake detection
     * @param
     */
     virtual void addReport(const Report &r) = 0;
     virtual void addReports(const std::vector<Report> rs) = 0;

     virtual int size() = 0;
     virtual void clear() = 0;

     virtual std::string getDetectorName() = 0;

 protected:
    virtual void elaborateActualReports() = 0;
    virtual Event generateEventFromReport(const Report &r)  {
        Event e;
        e.isRealTimeReport = true;
        e.id = TimeUtils::getCurrentMillis();
        e.millis = r.millis;
        e.date = TimeUtils::getTimeStringFromMillis(r.millis);
        e.author = getDetectorName();
        e.magnitude = 10;
        e.lat = r.u.lat;
        e.lng = r.u.lng;
        return e;
    }
    double getDistance(const Report &a, const Report &b) {
        return GeoUtility::distanceEarth(a.u.lat, a.u.lng, b.u.lat, b.u.lng);
    }
};


#endif //SIMPLE_WEB_SERVER_DETECTOR_H
