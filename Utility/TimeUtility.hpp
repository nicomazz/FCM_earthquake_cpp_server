//
// Created by nicomazz97 on 21/11/16.
//

#ifndef SIMPLE_WEB_SERVER_TIME_H
#define SIMPLE_WEB_SERVER_TIME_H

#include <chrono>
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"


namespace TimeUtility {
    const long MILLIS_IN_MINUTE = 1000 * 60;
    const long MILLIS_IN_A_HOUR = MILLIS_IN_MINUTE * 60;
    const long MILLIS_IN_A_DAY = MILLIS_IN_A_HOUR * 24;

    long getCurrentMillis();

    bool isInLastHour(long millis);

    long getNDaysAgoMillis(int days);

    unsigned long long getMillisFromTimeString(std::string s);

    std::string getTimeStringFromMillis(unsigned long long millis);

    boost::posix_time::ptime epoch_milliseconds_to_ptime(unsigned long long ms);
}
#endif //SIMPLE_WEB_SERVER_TIMEUTILITY_H
