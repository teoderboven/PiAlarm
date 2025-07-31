cmake_minimum_required(VERSION 3.27)

project(PiAlarm_media LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        MusicPlayer.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

# link needed project libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
        bass
)
target_link_libraries(${PROJECT_NAME} PUBLIC
        PiAlarm_logging
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Copy the BASS runtime library to the output directory
get_target_property(BASS_RUNTIME_PATH bass IMPORTED_LOCATION)

# Check if the BASS runtime path exists and is valid
if(EXISTS "${BASS_RUNTIME_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${BASS_RUNTIME_PATH}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    message(WARNING "IMPORTED_LOCATION for `bass` is not set or does not exist: ${BASS_RUNTIME_PATH}")
endif()
