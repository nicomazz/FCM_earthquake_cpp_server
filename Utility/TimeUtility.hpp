//
// Created by nicomazz97 on 21/11/16.
//

#ifndef SIMPLE_WEB_SERVER_TIME_H
#define SIMPLE_WEB_SERVER_TIME_H

#include <chrono>
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"


namespace TimeUtils{
    long getCurrentMillis();
    std::string getTimeStringFromMillis(unsigned long long millis);
    boost::posix_time::ptime epoch_milliseconds_to_ptime(unsigned long long ms);
}
#endif //SIMPLE_WEB_SERVER_TIMEUTILITY_H
