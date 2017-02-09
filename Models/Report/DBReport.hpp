//
// Created by nicomazz97 on 10/12/16.
//

#ifndef SIMPLE_WEB_SERVER_DBREPORT_HPP
#define SIMPLE_WEB_SERVER_DBREPORT_HPP

#pragma db object

class DBReport {
public:
    DBReport(){lat = lng = 0;}
#pragma db id
    long millis;
    int user_id;
    int power;

    double lat;
    double lng;

};


#endif //SIMPLE_WEB_SERVER_DBREPORT_HPP
