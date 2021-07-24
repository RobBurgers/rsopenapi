cmake_minimum_required(VERSION 3.5.1)

project(rsopenapi)

find_package(RtDB)
find_package(Boost COMPONENTS thread program_options REQUIRED QUIET)

set(CMAKE_CXX_STANDARD 17) # for std::filesystem
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Werror")

include_directories("include")

add_library(rsopenapi
    impl/Robot.cpp
    impl/Service.cpp
)
add_library(RS::rsopenapi ALIAS rsopenapi)
target_link_libraries(rsopenapi 
    RtDB::comm
    RtDB::rtdb
    Boost::thread
)
target_include_directories(rsopenapi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

