FCM-earthquake-cpp-server [![Build Status](https://travis-ci.org/nicomazz/earthquake_server.svg?branch=master)](https://travis-ci.org/nicomazz/earthquake_server)
=================

[This was a Fork of https://github.com/eidheim/Simple-Web-Server]

Implementation of a server-side C++ application to manage the notification about earthquakeS in an android app
using Firebase Cloud Messaging.
The server receives the report of probable earthquakes by the net of android devices and tries to understand if a real earthquake is happening.
If it is, the server sends a real-time notification to every near users. If the earthquake is detected at the epicenter, the server can notify users some seconds before they will feel the quake.

### Usage
Create eq.service in '/usr/lib/systemd/system' with this content:

```sh
[Unit]
Description=eqserver

[Service]
Type=forking
ExecStart=/path/to/eqarthquake_server
```

then to enable and manage the service:

```sh
systemctl enable eq
systemctl start/stop/status eq
```


### Dependencies

* Boost C++ libraries

### Compile and run

Compile with a C++11 compliant compiler:
```sh
mkdir build
cd build
cmake ..
make
cd ..
```

#### HTTP

Run the server and client examples: `./build/http_web_server`
Direct your favorite browser to for instance http://localhost:8080/
To view the logs : "tail -f /var/log/syslog | grep eqserverd"
