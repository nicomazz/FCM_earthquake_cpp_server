//
// Created by nicomazz97 on 02/12/16.
//

#ifndef SIMPLE_WEB_SERVER_GEOUTILITY_HPP
#define SIMPLE_WEB_SERVER_GEOUTILITY_HPP

#define PI 3.14159265358979323846
#define earthRadiusKm 6371.0

class GeoUtility {
public:
    static double deg2rad(const double deg);

    static double rad2deg(const double rad);

    /**
    * Returns the distance between two points on the Earth.
    * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
    * @param lat1d Latitude of the first point in degrees
    * @param lon1d Longitude of the first point in degrees
    * @param lat2d Latitude of the second point in degrees
    * @param lon2d Longitude of the second point in degrees
    * @return The distance between the two points in kilometers
    */
    static double distanceEarth(const double lat1d, const double lon1d, const double lat2d, const double lon2d);

    static int getQuakeRangeKm(double magnitude);
};


#endif //SIMPLE_WEB_SERVER_GEOUTILITY_HPP
