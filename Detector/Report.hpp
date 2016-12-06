#include <Utility/TimeUtility.hpp>
#include "../Models/User/User.hpp"
#ifndef SIMPLE_WEB_REPORT
#define SIMPLE_WEB_REPORT

#define REPORT_USER_ID "UserId"
#define REPORT_SECRET_KEY "FirebaseId"
#define REPORT_POWER "power"

struct Report{
    Report(){millis = TimeUtils::getCurrentMillis();}
    Report(long _millis){millis = _millis;}
    Report(User & _user, int _power):u(_user),power(_power){
        millis = TimeUtils::getCurrentMillis();
    }
	User u;
    int power;
	long millis;
	bool operator<(const Report & b) const{
		return millis < b.millis;
	}
    bool operator==(const Report & b)const{
        return millis == b.millis;
    }
};

#endif
