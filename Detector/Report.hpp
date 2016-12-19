
#ifndef SIMPLE_WEB_REPORT
#define SIMPLE_WEB_REPORT

#include <Utility/TimeUtility.hpp>
#include "../Models/User/User.hpp"

#define REPORT_USER_ID "userId"
#define REPORT_SECRET_KEY "secretKey"
#define REPORT_POWER "power"

struct Report{
    Report(){millis = TimeUtility::getCurrentMillis();}
    Report(long _millis){millis = _millis;}
    Report(User & _user, int _power):u(_user),power(_power){
        millis = TimeUtility::getCurrentMillis();
    }
	User u;
    int power;
#pragma db id
    long millis;
	bool operator<(const Report & b) const{
		return millis < b.millis;
	}
    bool operator==(const Report & b)const{
        return millis == b.millis;
    }
};

#endif
