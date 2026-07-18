cmake_minimum_required(VERSION 3.27)

project(PiAlarm_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(manager)

set(HEADERS
        Alarm.hpp
        AlarmsData.hpp
        AlarmState.hpp
        BaseModelData.hpp
        ClockData.hpp
        CO2Data.hpp
        CurrentIndoorData.hpp
        CurrentWeatherData.h
        Time.h
)

set(SOURCES
        Alarm.cpp
        AlarmsData.cpp
        AlarmState.cpp
        ClockData.cpp
        CO2Data.cpp
        CurrentIndoorData.cpp
        CurrentWeatherData.cpp
        Time.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_common
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
