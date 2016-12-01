//
// Created by nicomazz97 on 20/11/16.
//

#ifndef SIMPLE_WEB_SERVER_DETECTOR_H
#define SIMPLE_WEB_SERVER_DETECTOR_H

#include <set>

#include "Shake.hpp"
#include "../Utility/LatLng.hpp"

#define SHAKE_TTL_MS (1000)*(60)*(5)

class EQDetector {
public:
	/**
	 * Add a user shake detection
	 * @param
	 */
	void addShake(User u);

private:
	std::set<Shake> shakes;
	void elaborateActualShakes();
	/**
	 * remove shakes how are older than a constant
	 */
	void removeOldShakes();

	Event generateEarthquakeAlert();

	LatLng getMediumPoint(std::vector<LatLng> positions);


};


#endif //SIMPLE_WEB_SERVER_DETECTOR_H
