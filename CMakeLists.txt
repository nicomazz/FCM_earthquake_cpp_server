cmake_minimum_required (VERSION 2.8.7)
project (Simple-Web-Server)
#SET(CMAKE_CXX_COMPILER gcc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Wno-unknown-pragmas -Wno-format-security -L/usr/local/lib -L/usr/lib64 -lstdc++")

# il problema era che non riusciva ad effettuare il linking perchè non aveva la directort alle librerie, che era -L/usr/local/lib

include_directories(.)

find_package(Threads REQUIRED)

set(BOOST_COMPONENTS system thread filesystem date_time)
# Late 2017 TODO: remove the following checks and always use std::regex
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        set(BOOST_COMPONENTS ${BOOST_COMPONENTS} regex)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_BOOST_REGEX")
    endif()
endif()

find_package(Boost 1.53.0 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()



file(GLOB DataSources_dir  "DataSources/*.cpp")
file(GLOB EventsWebProvider_dir  "DataSources/*/*.cpp")
file(GLOB Models_dir "Models/*/*")
file(GLOB Firebase_dir "Firebase/*.cpp")
file(GLOB Detector_dir "Detector/*")
file(GLOB Utility_dir "Utility/*")

add_executable(http_web_server
        http_web_server.cpp
        EarthquakeServer.cpp
        ServerUtility/FirecloudServerInitializer.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir} ${Detector_dir} ${Utility_dir}
        DataSources/Database.hpp
        Utility/UserMatching.cpp Utility/UserMatching.hpp Utility/json.hpp
        Utility/TimeUtility.cpp)

#MESSAGE( STATUS "dir1 sources:         " ${Dir1_Sources} )


#TODO: add requirement for version 1.0.1g (can it be done in one line?)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(http_web_server ${OPENSSL_LIBRARIES})
endif()


target_link_libraries(http_web_server
        odb-sqlite
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        odb
        )


if(MSYS) #TODO: Is MSYS true when MSVC is true?
    target_link_libraries(http_web_server ws2_32 wsock32)
endif()

enable_testing()
add_subdirectory(tests)

#to add secret firebase secret key
if(EXISTS Firebase/secret_key.txt)
    configure_file(Firebase/secret_key.txt secret_key.txt COPYONLY)
endif()

install(FILES ServerUtility/server_http.hpp DESTINATION include/simple-web-server)
