cmake_minimum_required (VERSION 2.8.8)
project (Simple-Web-Server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

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



file(GLOB DataSources_dir  "DataSources/*.cpp")
file(GLOB Models_dir "Models/*.cpp")

add_executable(http_web_server http_web_server.cpp ${DataSources_dir} ${Models_dir} DataSources/DataSource.h)
#MESSAGE( STATUS "dir1 sources:         " ${Dir1_Sources} )
#MESSAGE( STATUS "dir2 sources:         " ${Dir2_Sources} )




target_link_libraries(http_web_server ${Boost_LIBRARIES})
target_link_libraries(http_web_server ${CMAKE_THREAD_LIBS_INIT})

if(MSYS) #TODO: Is MSYS true when MSVC is true?
    target_link_libraries(http_web_server ws2_32 wsock32)
endif()

enable_testing()
add_subdirectory(tests)

install(FILES server_http.hpp  DESTINATION include/simple-web-server)
