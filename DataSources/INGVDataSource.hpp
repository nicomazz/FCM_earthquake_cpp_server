#include "DataSource.h"

// todo
class INGVDataSource : DataSource{
public:
    virtual std::vector<Event> requestEvents() override;

private:
    virtual Event buildEvent(std::string s) override;
    virtual std::string getWebServiceUrl() override;
    virtual std::vector<Event> parseEvents(std::string webResponse) override;
};