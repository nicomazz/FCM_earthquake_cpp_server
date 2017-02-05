//
// Created by nicomazz97 on 26/11/16.
//

#ifndef SIMPLE_WEB_SERVER_EARTQUAKESERVER_H
#define SIMPLE_WEB_SERVER_EARTQUAKESERVER_H

#include <syslog.h>

#include <DataSources/EventProvider.hpp>
#include <Firebase/FirebaseNotificationManager.hpp>
#include <ServerUtility/client_http.hpp>
#include <ServerUtility/server_http.hpp>
#include <ServerUtility/HTTPQuakeServer.hpp>

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

/**
 * main server class. From this class all the different part of the server will start:
 * - Notify server: continuously parse events from the web source, and handle user notification
 * - Http server: responds to app request, manage the reports receive
 * - Event related reports: every 10 minutes seeks for new official events and try to allocate to these reports
 */
class EarthquakeServer {
public:
    /**
     * init http server and starts other parts
     */
    void startServer();

private:

    /**
     * notify server
     */
    void serverMainLoop();
    void searchForEventsToNotify();

    /**
     * check the correlation between report and last hour's events
     */
    void eventRelatedEventLoop();
};


#endif //SIMPLE_WEB_SERVER_EARTQUAKESERVER_H
