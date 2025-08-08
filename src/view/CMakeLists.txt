cmake_minimum_required(VERSION 3.27)

project(PiAlarm_view LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include subdirectory for views implementation and manager
add_subdirectory(manager)
add_subdirectory(console)

set(SOURCES
        AbstractMainClockView.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
