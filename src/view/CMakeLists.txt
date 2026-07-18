cmake_minimum_required(VERSION 3.27)

project(PiAlarm_view LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include subdirectory for views implementation and manager
add_subdirectory(manager)
add_subdirectory(console)
add_subdirectory(ssd1322)

set(HEADERS
        AbstractMainClockView.h
        AbstractObserverView.h
        IView.h
)

set(SOURCES
        AbstractMainClockView.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_model
        PiAlarm_common
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
