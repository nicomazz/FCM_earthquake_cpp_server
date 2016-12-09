set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-access-control -std=c++0x -Wall -Wextra -Wno-unknown-pragmas -Wno-format-security -L/usr/local/lib -L/usr/lib64 -lstdc++" )

#IO TEST

add_executable(io_test io_test.cpp )
target_link_libraries(io_test ${Boost_LIBRARIES})
target_link_libraries(io_test ${CMAKE_THREAD_LIBS_INIT})

add_executable(parse_test parse_test.cpp)
target_link_libraries(parse_test ${Boost_LIBRARIES})
target_link_libraries(parse_test ${CMAKE_THREAD_LIBS_INIT})

# for faster add_executable

file(GLOB Models_dir "../Models/*/*")
file(GLOB Utility_dir "../Utility/*")
file(GLOB DataSources_dir  "../DataSources/*.cpp")
file(GLOB EventsWebProvider_dir  "../DataSources/*/*.cpp")
file(GLOB Firebase_dir "../Firebase/*.cpp")
file(GLOB Detector_dir "../Detector/*")

#EVENT PROVIDER TESTS
add_executable(event_provider_test event_provider_test.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir} ${Detector_dir} ${Utility_dir}
        ../DataSources/Database.cpp
        ../DataSources/EventProvider.cpp
        ../DataSources/EventsWebProvider/INGVDataSource.cpp
        ../DataSources/EventsWebProvider/WebDataSourceInterface.cpp
        ../Utility/UserMatching.cpp ../Utility/UserMatching.hpp
        ../Detector/SimpleEQDetector.cpp)

target_link_libraries(event_provider_test ${Boost_LIBRARIES})
target_link_libraries(event_provider_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(event_provider_test odb odb-sqlite)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(event_provider_test ${OPENSSL_LIBRARIES})
endif()

#USER PROVIDER TESTS

add_executable(user_provider_test user_provider_test.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir} ${Detector_dir} ${Utility_dir}
        ../DataSources/Database.cpp
        ../DataSources/UserPreferenceProvider.cpp
        ../Utility/UserMatching.cpp ../Utility/UserMatching.hpp
        ../Detector/SimpleEQDetector.cpp)

target_link_libraries(user_provider_test ${Boost_LIBRARIES})
target_link_libraries(user_provider_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(user_provider_test odb odb-sqlite)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(user_provider_test ${OPENSSL_LIBRARIES})
endif()
#USER SERVER TEST

add_executable(user_server_test
        user_server_test.cpp
        ../EarthquakeServer.cpp
        ../ServerUtility/ServerInitializer.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir} ${Detector_dir} ${Utility_dir}
        ../DataSources/Database.hpp
        ../Utility/UserMatching.cpp
        ../Utility/UserMatching.hpp
        ../Utility/json.hpp
        ../Utility/TimeUtility.cpp)

target_link_libraries(user_server_test ${Boost_LIBRARIES})
target_link_libraries(user_server_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(user_server_test odb odb-sqlite)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(user_server_test ${OPENSSL_LIBRARIES})
endif()

#SIMPLE EQDETECTOR TESTS

add_executable(simple_detector_test
        simple_detector_test.cpp
        ../EarthquakeServer.cpp
        ../ServerUtility/ServerInitializer.cpp
        ${DataSources_dir} ${Models_dir} ${Firebase_dir} ${EventsWebProvider_dir} ${Detector_dir} ${Utility_dir}
        ../DataSources/Database.hpp
        ../Utility/UserMatching.cpp
        ../Utility/UserMatching.hpp
        ../Utility/json.hpp
        ../Utility/TimeUtility.cpp)

target_link_libraries(simple_detector_test ${Boost_LIBRARIES})
target_link_libraries(simple_detector_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(simple_detector_test odb odb-sqlite)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(simple_detector_test ${OPENSSL_LIBRARIES})
endif()

########################################

if(MSYS) #TODO: Is MSYS true when MSVC is true?
    target_link_libraries(io_test ws2_32 wsock32)
    target_link_libraries(parse_test ws2_32 wsock32)
    target_link_libraries(event_provider_test ws2_32 wsock32)
endif()

add_test(io_test io_test)
add_test(parse_test parse_test)
add_test(event_provider_test event_provider_test)
add_test(user_server_test user_server_test)
add_test(simple_detector_test simple_detector_test)