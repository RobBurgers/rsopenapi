cmake_minimum_required(VERSION 3.5.1)

project(rsopenapi)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(RTDB_CONFIG "rtdb_configuration.xml")
configure_file("config/${RTDB_CONFIG}" "${CMAKE_BINARY_DIR}/config/${RTDB_CONFIG}" COPYONLY)

enable_testing()

add_subdirectory(rtdb2)
add_subdirectory(src)
add_subdirectory(example)
