//
// Created by nicomazz97 on 04/02/17.
//

#ifndef SIMPLE_WEB_SERVER_REPORTBUILDER_HPP
#define SIMPLE_WEB_SERVER_REPORTBUILDER_HPP


#include <Firebase/FirebaseNotificationManager.hpp>
#include <Models/Report/DBReport-odb.hpp>


class ReportBuilder {
public:
    static json reportToJson(DBReport & r);
    static json reportsToJson(std::vector<DBReport> & reports);
};


#endif //SIMPLE_WEB_SERVER_REPORTBUILDER_HPP
