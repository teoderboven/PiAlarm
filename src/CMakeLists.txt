cmake_minimum_required(VERSION 3.27)

project(PiAlarm VERSION 0.1 LANGUAGES CXX)

# Set compiler flags to treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libstdc++ -static-libgcc")


option(DISPLAY_SSD1322 "Use SSD1322 display" OFF)
option(DISPLAY_CONSOLE "Use console display" OFF)

if(DISPLAY_SSD1322 AND DISPLAY_CONSOLE)
    message(FATAL_ERROR "You cannot enable both DISPLAY_SSD1322 and DISPLAY_CONSOLE.")
elseif(NOT DISPLAY_SSD1322 AND NOT DISPLAY_CONSOLE)
    message(FATAL_ERROR "You must enable one display option: DISPLAY_SSD1322 or DISPLAY_CONSOLE.")
endif()

# global definitions based on display options
if(DISPLAY_SSD1322)
    add_compile_definitions(DISPLAY_SSD1322)
elseif(DISPLAY_CONSOLE)
    add_compile_definitions(DISPLAY_CONSOLE)
endif()

add_subdirectory(model)
add_subdirectory(view)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_model
        PiAlarm_view_manager
        PiAlarm_view_cli
)
