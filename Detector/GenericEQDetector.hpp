//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DETECTOR_H
#define SIMPLE_WEB_SERVER_DETECTOR_H

#include "../Models/User/User.hpp"
#include "Report.hpp"

#define SHAKE_TTL_MS (1000)*(60)*(5)


class GenericEQDetector {
public:
	/**
	 * Add a user shake detection
	 * @param
	 */
	virtual void addReports(const Report &r) = 0;

protected:
    virtual void elaborateActualReports() = 0;



};


#endif //SIMPLE_WEB_SERVER_DETECTOR_H
