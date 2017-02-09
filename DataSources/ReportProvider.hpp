//
// Created by nicomazz97 on 10/12/16.
//

#ifndef SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
#define SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP


#include <Detector/Report.hpp>
#include <Models/Report/DBReport-odb.hpp>
#include <Models/Event/Event.hpp>

class ReportProvider {
public:
    static long persistReport(const Report &r);

    static std::vector<DBReport> getReportsFromToTime(long from_millis, long to_millis);

    /**
     * take all reports that could be related to the event happened at millis_event
     * from millis_event - 1 minute to millis_event + 5 minutes
     * @param millis_event
     * @return
     */
    static std::vector<DBReport> getReportsRelatedToEvents(Event &e);

    static void deleteReport(Report &r);

private:
    static void getReportPosition(DBReport &r, double &lat, double &lng);
};

#endif //SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
