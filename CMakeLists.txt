cmake_minimum_required(VERSION 3.27)

project(PiAlarm)

include_directories(include)

# Include project dependencies
include(cmake/Dependencies.cmake)

# Include subdirectories for source
add_subdirectory(src)
