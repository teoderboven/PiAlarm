cmake_minimum_required(VERSION 3.27)

project(PiAlarm_controller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        AlarmController.h
)

set(SOURCES
        AlarmController.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_model
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
