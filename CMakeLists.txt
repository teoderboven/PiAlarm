cmake_minimum_required(VERSION 3.27)

project(PiAlarm)

include_directories(include)

# Include project dependencies
include(cmake/Dependencies.cmake)

# Include dependencies not compatible with basic CMake
add_subdirectory(external)

# Include subdirectories for source
add_subdirectory(src)

# Include subdirectories for tests
add_subdirectory(tests)
