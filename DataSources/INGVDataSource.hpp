#include "WebDataSourceInterface.hpp"

class INGVDataSource : public WebDataSourceInterface{
private:
    /**
     * costruisce un evento a partire da una stringa
     * @param s
     * @return
     */
    virtual Event buildEvent(std::string s) override;
    /**
     * ritorna l'url al quale fare la richiesta
     * @return
     */
    virtual std::string getWebServiceUrl() override;
    /**
     * gestisce la risposta del web-service
     * @param webResponse
     * @return
     */
    virtual std::vector<Event> parseEvents(std::string webResponse) override;

    virtual std::string getWebServiceUrlParams() override;

    ~INGVDataSource(){}
};