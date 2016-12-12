#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string>
#include <limits.h>
#include <unistd.h>
#include <zconf.h>

#include "ServerUtility/server_http.hpp"
#include "ServerUtility/client_http.hpp"
#include "EarthquakeServer.hpp"


#define DAEMON_NAME "eqserverd"

//#define DEBUG

using namespace std;

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef SimpleWeb::Client<SimpleWeb::HTTP> HttpClient;

void runServer() {
    EarthquakeServer server;
    server.startServer();
}

/**
 * TODO:
 * mettere millis a eventi in database
 * -aggiungere al web server path per prendere solo i report generati
 * -localizzare tutto in inglese
 * -in simple detector prendere come condizione necessaria il 30 % degli attivi entro K Km, e consentire
 *  una sola segnalazione per utente (se ne invia di nuove la precedente viene a meno)
 * -aggiungere schermata nell'app per le segnalazioni inviate dagli utenti
 * -migliorare fragment detector
 * -verificare in caso il server sia spento cosa fanno i dispositivi
 * */
std::string getexepath() {
    char result[1000];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

std::string getCurrentDirectory() {
    string s1(getexepath());
    s1 = s1.substr(0, s1.find_last_of("\\/"));
    return s1;
}

int main() {
    openlog(DAEMON_NAME, 0, LOG_USER);

#ifdef DEBUG
    syslog(LOG_INFO, "Starting Earthquake web server!");
#else
    syslog(LOG_INFO, "Entering Daemon");

    pid_t pid, sid;

    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }
    if (pid > 0) { exit(EXIT_SUCCESS); }
    umask(0);
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //change path to the executable one
    if ((chdir(getCurrentDirectory().c_str())) < 0) { exit(EXIT_FAILURE); }

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
