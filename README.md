Simple-Web-Server [![Build Status](https://travis-ci.org/eidheim/Simple-Web-Server.svg?branch=master)](https://travis-ci.org/eidheim/Simple-Web-Server)
=================

Fork of https://github.com/eidheim/Simple-Web-Server to implement a c++ Web-server to manage Firebase Cloud Messaging.



A very simple, fast, multithreaded, platform independent HTTP and HTTPS server and client library implemented using C++11 and Boost.Asio. Created to be an easy way to make REST resources available from C++ applications. 


###Usage

See http_web_server.cpp or https_examples.cpp for example usage. 

See particularly the JSON-POST (using Boost.PropertyTree) and the GET /match/[number] examples, which are most relevant.

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

Run the server and client examples: `./build/https_examples`

Direct your favorite browser to for instance https://localhost:8080/

