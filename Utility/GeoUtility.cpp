//
// Created by nicomazz97 on 02/12/16.
//

#include <cmath>
#include "GeoUtility.hpp"

// This function converts decimal degrees to radians
double GeoUtility::deg2rad(const double deg) {
    return (deg * PI / 180);
}

//  This function converts radians to decimal degrees
double GeoUtility::rad2deg(const double rad) {
    return (rad * 180 / PI);
}

double GeoUtility::distanceEarth(const double lat1d, const double lon1d, const double lat2d, const double lon2d) {

    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r) / 2);
    v = sin((lon2r - lon1r) / 2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));

}
