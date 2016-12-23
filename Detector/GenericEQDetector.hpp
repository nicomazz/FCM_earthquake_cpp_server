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
    virtual Event generateEventFromReports(const std::vector<Report> & reports)  {
        Report r = reports[0];
        Event e;
        e.isRealTimeReport = true;
        e.id = TimeUtility::getCurrentMillis();
        e.millis = r.millis;
        e.date = TimeUtility::getTimeStringFromMillis(r.millis);
        e.author = getDetectorName();
        e.magnitude = 10;
        setMeanPosition(reports,e);
        e.contributorId = getIdsString(reports);
        return e;
    }
    void setMeanPosition(const std::vector<Report> & reports, Event & e){
        double lat_sum = 0, lng_sum = 0;
        int cnt = 0;
        for (const Report & r: reports)
            if (r.u.lat != 0.0)
                lat_sum += r.u.lat, lng_sum+= r.u.lng, cnt++;
        e.lat = lat_sum / cnt;
        e.lng = lng_sum / cnt;
    }
    std::string getIdsString( const std::vector<Report> & reports){
        json ids;
        for (Report r: reports)
            if (r.u.lat != 0.0)
                ids.push_back(r.u.id);
        return ids.dump();
    }
    double getDistance(const Report &a, const Report &b) {
        return getDistance(a.u,b.u);
    }
    double getDistance(const User &a, const User &b) {
        return GeoUtility::distanceEarth(a.lat, a.lng, b.lat, b.lng);
    }
};


#endif //SIMPLE_WEB_SERVER_DETECTOR_H
