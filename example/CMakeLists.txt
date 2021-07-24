cmake_minimum_required(VERSION 3.5.1)

project(rsopenapi)

# system dependencies
find_package(RtDB)
find_package(Boost REQUIRED COMPONENTS program_options QUIET)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# binary
add_executable(example example.cpp)
target_link_libraries(example
    rsopenapi
    RtDB::rtdb
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
