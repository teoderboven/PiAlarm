cmake_minimum_required(VERSION 3.27)

project(PiAlarm_trigger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        AlarmSoundTrigger.h
)

set(SOURCES
        AlarmSoundTrigger.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

# link project libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_media
        PiAlarm_model
        PiAlarm_common
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
