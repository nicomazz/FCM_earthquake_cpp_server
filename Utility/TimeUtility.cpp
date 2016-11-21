//
// Created by nicomazz97 on 21/11/16.
//

#include "TimeUtility.hpp"


long TimeUtils::getCurrentMillis(){
    using namespace std::chrono;
    return duration_cast<milliseconds>
            (system_clock::now().time_since_epoch()).count();
}