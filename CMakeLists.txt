cmake_minimum_required (VERSION 2.8.7)
project (Simple-Web-Server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unknown-pragmas")

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

find_package(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
ENDIF()

#find_package(Boost 1.53.0 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
#include_directories(SYSTEM ${Boost_INCLUDE_DIR})

if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()



file(GLOB DataSources_dir  "DataSources/*.cpp")
file(GLOB EventsWebProvider_dir  "DataSources/*/*.cpp")
file(GLOB Models_dir "Models/*/*.cpp")
file(GLOB Firebase_dir "Firebase/*.cpp")

add_executable(http_web_server
        http_web_server.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir}
        DataSources/Database.hpp
        Utility/UserMatching.cpp Utility/UserMatching.hpp Utility/json.hpp Utility/TimeUtility.hpp
        FirecloudServerInitializer.h Firebase/secretKey.h Utility/TimeUtility.cpp)

#MESSAGE( STATUS "dir1 sources:         " ${Dir1_Sources} )
MESSAGE( STATUS "boost lib         " ${Boost_LIBRARIES} )


#TODO: add requirement for version 1.0.1g (can it be done in one line?)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(http_web_server ${OPENSSL_LIBRARIES})
endif()


target_link_libraries(http_web_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        odb
        odb-sqlite
        )


if(MSYS) #TODO: Is MSYS true when MSVC is true?
    target_link_libraries(http_web_server ws2_32 wsock32)
endif()

enable_testing()
add_subdirectory(tests)

install(FILES server_http.hpp  DESTINATION include/simple-web-server)
