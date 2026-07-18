cmake_minimum_required(VERSION 3.27)

project(PiAlarm_view_ssd1322 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        AlarmsSettingsView.h
        MainClockView.h
)

set(SOURCES
        AlarmsSettingsView.cpp
        MainClockView.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_gfx
        PiAlarm_utils
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)
