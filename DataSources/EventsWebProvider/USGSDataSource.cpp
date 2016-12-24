//
// Created by nicomazz97 on 24/12/16.
//

#include "USGSDataSource.hpp"

std::string USGSDataSource::getDataSourceName(){
    return "USGS";
}
std::string USGSDataSource::getWebServiceUrl() {
    return "http://earthquake.usgs.gov/fdsnws/event/1/query?format=text";
}
