//
// Created by nicomazz97 on 21/11/16.
//

#include "TimeUtility.hpp"


long TimeUtils::getCurrentMillis(){
    using namespace std::chrono;
    return duration_cast<milliseconds>
            (system_clock::now().time_since_epoch()).count();
}

std::string TimeUtils::getTimeStringFromMillis(unsigned long long millis) {
    boost::posix_time::ptime t = epoch_milliseconds_to_ptime(millis);
    return boost::posix_time::to_iso_extended_string(t);
}
boost::posix_time::ptime TimeUtils::epoch_milliseconds_to_ptime(unsigned long long ms)
{
    static const  boost::posix_time::ptime epoch( boost::gregorian::date(1970, 1, 1));
    return epoch +  boost::posix_time::milliseconds(ms);
}
