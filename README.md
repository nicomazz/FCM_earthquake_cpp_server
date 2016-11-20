Simple-Web-Server [![Build Status](https://travis-ci.org/nicomazz/Simple-Web-Server.svg?branch=master)](https://travis-ci.org/eidheim/Simple-Web-Server)
=================

[This was a Fork of https://github.com/eidheim/Simple-Web-Server]

Implementation of a server-side C++ application to manage the notification about earthquake in an android app
using Firebase Cloud Messaging.

###Usage

See http_web_server.cpp or https_examples.cpp for example usage. 

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
