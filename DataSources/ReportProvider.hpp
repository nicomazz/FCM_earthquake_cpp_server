//
// Created by nicomazz97 on 10/12/16.
//

#ifndef SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
#define SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP


#include <Detector/Report.hpp>

class ReportProvider {
public:
    static long persistReport(const Report &r);
};

#endif //SIMPLE_WEB_SERVER_REPORTPROVIDER_HPP
