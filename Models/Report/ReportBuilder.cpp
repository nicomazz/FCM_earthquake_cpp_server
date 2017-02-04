//
// Created by nicomazz97 on 04/02/17.
//

#include "ReportBuilder.hpp"

 json ReportBuilder::reportToJson(DBReport & r){
     json att;
     att["userId"] = r.user_id;
     att["millis"] = r.millis;
     att["power"] = r.power;
     User u = UserPreferenceProvider::getUser(r.user_id);
     att["lat"] = u.lat;
     att["lng"] = u.lng;
     att["dateTime"] = TimeUtility::getTimeStringFromMillis(r.millis);
     return att;
 }

 json ReportBuilder::reportsToJson(std::vector<DBReport> & reports){
     json result = json::array();
     for (DBReport & r : reports)
         result.push_back(reportToJson(r));
     return result;
 }
