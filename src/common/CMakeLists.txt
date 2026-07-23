cmake_minimum_required(VERSION 3.27)

project(PiAlarm_common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        HasWorker.cpp
        HasWorker.h
        Observable.hpp
        Observer.h
        WeatherCondition.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        PiAlarm_logging
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
