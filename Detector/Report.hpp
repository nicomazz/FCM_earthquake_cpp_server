#include <Utility/TimeUtility.hpp>
#include "../Models/User/User.hpp"
#ifndef SIMPLE_WEB_REPORT
#define SIMPLE_WEB_REPORT

struct Report{
    Report(){millis = TimeUtils::getCurrentMillis();}
    Report(long _millis){millis = _millis;}
    Report(User & _user, int _power):u(_user),power(_power){}
	User u;
    int power;
	long millis;
	bool operator<(const Report & b) const{
		return millis > b.millis;
	}
    bool operator==(const Report & b)const{
        return millis == b.millis;
    }
};

#endif
