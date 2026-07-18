cmake_minimum_required(VERSION 3.27)

project(PiAlarm_service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        BaseService.h
        BME280Service.h
        IService.h
        SCD41Service.h
        TimeUpdateService.h
        WeatherApiService.h
)

set(SOURCES
        BaseService.cpp
        BME280Service.cpp
        SCD41Service.cpp
        TimeUpdateService.cpp
        WeatherApiService.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

# link needed project libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_logging
        PiAlarm_provider
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
