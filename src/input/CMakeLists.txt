cmake_minimum_required(VERSION 3.27)

project(PiAlarm_input LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        HasInputEventHandler.h
        InputEvent.h
        InputManager.h
)

set(SOURCES
        InputManager.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
