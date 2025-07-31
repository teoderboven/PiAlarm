cmake_minimum_required(VERSION 3.27)

project(Bass_test VERSION 1.0 LANGUAGES CXX)

# Set compiler flags to treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libstdc++ -static-libgcc")


add_executable(${PROJECT_NAME}
        bassTest.cpp
)

# Link the executable with the necessary libraries
target_link_libraries(${PROJECT_NAME} bass)

copy_bass_dll_to_target(${PROJECT_NAME})
