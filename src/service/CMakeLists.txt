cmake_minimum_required(VERSION 3.27)

project(PiAlarm_service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        BaseService.cpp
        TimeUpdateService.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
