//
// Created by nicomazz97 on 24/12/16.
//

#ifndef SIMPLE_WEB_SERVER_USGSDATASOURCE_HPP
#define SIMPLE_WEB_SERVER_USGSDATASOURCE_HPP


#include "INGVDataSource.hpp"

class USGSDataSource : public INGVDataSource{
public:
    std::string getDataSourceName() override;
protected:
    /**
     * ritorna l'url al quale fare la richiesta
     * @return
     */
    std::string getWebServiceUrl() override;
};


#endif //SIMPLE_WEB_SERVER_USGSDATASOURCE_HPP
