//
// Created by nicomazz97 on 10/12/16.
//

#ifndef SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
#define SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP


#include <Detector/Report.hpp>
#include <Models/Report/DBReport-odb.hpp>
class ReportProvider {
public:
    static long persistReport(const Report &r);
    static std::vector<DBReport> getReportsFromToTime(long from_millis, long to_millis);
    static void deleteReport( Report &r);

};

#endif //SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
