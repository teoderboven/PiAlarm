cmake_minimum_required(VERSION 3.27)

project(PiAlarm_logging LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        HasLogger.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

# link external library
target_link_libraries(${PROJECT_NAME} PUBLIC
        spdlog::spdlog
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
