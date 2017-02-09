//
// Created by nicomazz97 on 21/11/16.
//

#include "TimeUtility.hpp"
#include <boost/date_time.hpp>


long TimeUtility::getCurrentMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>
            (system_clock::now().time_since_epoch()).count();
}

long TimeUtility::getNDaysAgoMillis(int days) {
    return getCurrentMillis() - MILLIS_IN_A_DAY * days;
}

bool TimeUtility::isInLastHour(long millis) {
    return getCurrentMillis() - millis <= MILLIS_IN_A_HOUR;
}


std::string TimeUtility::getTimeStringFromMillis(unsigned long long millis) {
    boost::posix_time::ptime t = epoch_milliseconds_to_ptime(millis);
    return boost::posix_time::to_iso_extended_string(t);
}

unsigned long long TimeUtility::getMillisFromTimeString(std::string s) {
    std::stringstream ss(s);
    boost::local_time::local_time_input_facet *ifc = new boost::local_time::local_time_input_facet();
    ifc->set_iso_extended_format();
    ss.imbue(std::locale(ss.getloc(), ifc));
    boost::local_time::local_date_time zonetime(boost::local_time::not_a_date_time);
    if (ss >> zonetime) {
        boost::posix_time::ptime t;
        t = zonetime.utc_time();
        boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
        boost::posix_time::time_duration const diff = t - epoch;
        return (unsigned long long int) diff.total_milliseconds();
    }
    return 0;
}

boost::posix_time::ptime TimeUtility::epoch_milliseconds_to_ptime(unsigned long long ms) {
    static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    return epoch + boost::posix_time::milliseconds(ms);
}

long TimeUtility::getPrecMidnightMillis(long millis) {
    return millis - (millis % MILLIS_IN_A_DAY);
}

long TimeUtility::getNextMidnightMillis(long millis) {
    return getPrecMidnightMillis(millis) + MILLIS_IN_A_DAY;
}
