cmake_minimum_required(VERSION 3.27)

project(PiAlarm_media LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        AudioTypes.h
        BassContext.hpp
        MusicPlayer.cpp
        MusicPlayer.h
        MusicService.cpp
        MusicService.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

# link needed project libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
        bass
        PiAlarm_logging
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
