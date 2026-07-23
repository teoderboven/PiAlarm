cmake_minimum_required(VERSION 3.27)

project(PiAlarm_view_console LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        MainClockView.cpp
        MainClockView.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_utils
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)
