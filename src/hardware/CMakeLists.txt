cmake_minimum_required(VERSION 3.27)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    return()
endif()

project(PiAlarm_hardware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD libgpiod)

if (NOT GPIOD_FOUND)
    message(FATAL_ERROR "libgpiod not found. Please install it via 'apt install libgpiod-dev'")
endif()

set(SOURCES
        GPIO.cpp
        SPI.cpp
        SSD1322.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the gpiod library
target_include_directories(${PROJECT_NAME} PUBLIC
        ${GPIOD_INCLUDE_DIRS}
)
link_directories(${GPIOD_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${GPIOD_LIBRARIES}
)
