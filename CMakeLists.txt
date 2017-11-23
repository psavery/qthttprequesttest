cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(httprequesttest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Network REQUIRED)

include_directories(${Qt5Network_INCLUDE_DIRS})

set(SRCS
    httprequesttest.cpp
    httprequest.cpp)

add_executable(httprequesttest ${SRCS})

target_link_libraries(httprequesttest Qt5::Network)
