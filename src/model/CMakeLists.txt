cmake_minimum_required(VERSION 3.27)

project(PiAlarm_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(manager)

set(SOURCES
        Time.cpp
        Alarm.cpp
        AlarmsData.cpp
        AlarmState.cpp
        ClockData.cpp
        CurrentIndoorData.cpp
        CurrentWeatherData.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
