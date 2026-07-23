cmake_minimum_required(VERSION 3.27)

project(PiAlarm_service LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        BaseService.h
        BME280Service.cpp
        BME280Service.h
        IService.h
        SCD41Service.cpp
        SCD41Service.h
        TimeUpdateService.cpp
        TimeUpdateService.h
        WeatherApiService.cpp
        WeatherApiService.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

# link needed project libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_common
        PiAlarm_provider
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
