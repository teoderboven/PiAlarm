cmake_minimum_required(VERSION 3.27)

project(Bass_test VERSION 1.0 LANGUAGES CXX)

# Set compiler flags to treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libstdc++ -static-libgcc")


add_executable(${PROJECT_NAME}
        bassTest.cpp
)

# Link the executable with the necessary libraries
target_link_libraries(${PROJECT_NAME} bass)

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
