cmake_minimum_required(VERSION 3.27)

project(PiAlarm VERSION 0.1 LANGUAGES CXX)

# Set compiler flags to treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libstdc++ -static-libgcc")


# Handle Raspberry Pi detection
option(FORCE_RASPBERRY_PI "Force Raspberry Pi target (cross-compiling)" OFF)

set(IS_RASPBERRY_PI OFF)

if(FORCE_RASPBERRY_PI)
    message(STATUS "Raspberry Pi forced by user")
    set(IS_RASPBERRY_PI ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux"
        AND EXISTS "/proc/device-tree/model")
    file(READ "/proc/device-tree/model" RPI_MODEL)
    if(RPI_MODEL MATCHES "Raspberry Pi")
        message(STATUS "Raspberry Pi detected: ${RPI_MODEL}")
        set(IS_RASPBERRY_PI ON)
    endif()
endif()


option(DISPLAY_SSD1322 "Use SSD1322 display (Raspberry Pi only)" OFF)
option(DISPLAY_CONSOLE "Use console display" OFF)
option(INPUT_GPIO "Use GPIO pin for user inputs (Raspberry Pi only)" OFF)
set(LOG_LEVEL "info" CACHE STRING "Log level (trace, debug, info, warn, error, critical)")

if(DISPLAY_SSD1322 AND DISPLAY_CONSOLE)
    message(FATAL_ERROR "You cannot enable both DISPLAY_SSD1322 and DISPLAY_CONSOLE.")
elseif(NOT DISPLAY_SSD1322 AND NOT DISPLAY_CONSOLE)
    message(FATAL_ERROR "You must enable one display option: DISPLAY_SSD1322 or DISPLAY_CONSOLE.")
endif()

if(DISPLAY_SSD1322 AND NOT IS_RASPBERRY_PI)
    message(FATAL_ERROR "DISPLAY_SSD1322 is only supported on Raspberry Pi.")
endif()

if(INPUT_GPIO AND NOT IS_RASPBERRY_PI)
    message(FATAL_ERROR "INPUT_GPIO is only supported on Raspberry Pi.")
endif()

# global definitions
if(IS_RASPBERRY_PI)
    add_compile_definitions(RASPBERRY_PI)
endif()

if(DISPLAY_SSD1322)
    add_compile_definitions(DISPLAY_SSD1322)
elseif(DISPLAY_CONSOLE)
    add_compile_definitions(DISPLAY_CONSOLE)
endif()

if(INPUT_GPIO)
    add_compile_definitions(INPUT_GPIO)
else()
    add_compile_definitions(INPUT_NONE)
endif()

add_subdirectory(controller)
add_subdirectory(gfx)
add_subdirectory(hardware)
add_subdirectory(input)
add_subdirectory(logging)
add_subdirectory(media)
add_subdirectory(model)
add_subdirectory(provider)
add_subdirectory(service)
add_subdirectory(trigger)
add_subdirectory(view)

add_executable(${PROJECT_NAME}
        main.cpp
        Application.cpp
)

# link project libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        PiAlarm_view_manager
        PiAlarm_view         # Contains the abstract view class and common functionality
        PiAlarm_controller
        PiAlarm_service
        PiAlarm_provider
        PiAlarm_trigger
        PiAlarm_model_manager
        PiAlarm_model
        PiAlarm_media
)

if(DISPLAY_SSD1322)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            PiAlarm_view_ssd1322
            PiAlarm_gfx
            PiAlarm_input
            PiAlarm_hardware
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
            PiAlarm_view_console
    )
endif()

copy_bass_dll_to_target(${PROJECT_NAME})
# Copy assets directory to the target directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

target_compile_definitions(PiAlarm_logging PRIVATE LOG_LEVEL="${LOG_LEVEL}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(BUILD_RELEASE=1)
endif()
