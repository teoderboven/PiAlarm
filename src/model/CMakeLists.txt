cmake_minimum_required(VERSION 3.27)

project(PiAlarm_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(manager)

set(HEADERS
)

set(SOURCES
        Alarm.cpp
        Alarm.hpp
        AlarmsData.cpp
        AlarmsData.hpp
        AlarmState.cpp
        AlarmState.hpp
        BaseModelData.hpp
        ClockData.cpp
        ClockData.hpp
        CO2Data.cpp
        CO2Data.hpp
        CurrentIndoorData.cpp
        CurrentIndoorData.hpp
        CurrentWeatherData.cpp
        CurrentWeatherData.h
        Time.cpp
        Time.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_common
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
