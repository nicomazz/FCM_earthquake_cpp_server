#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "EarthquakeServer.hpp"


#define DAEMON_NAME "eqserverd"

#define DEBUG

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

void runServer() {
    EarthquakeServer server;
    server.startServer();
}

int main() {
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
#ifdef DEBUG
    syslog(LOG_INFO, "Starting Earthquake web server!");
#else
    syslog(LOG_INFO, "Entering Daemon");

    pid_t pid, sid;

    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
   // if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#endif
    //----------------
    //Main Process
    //----------------
    runServer();

    //Close the log
    closelog();

    return 0;
}
