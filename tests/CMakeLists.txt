cmake_minimum_required(VERSION 3.27)

project(PiAlarm_tests VERSION 1.0 LANGUAGES CXX)

# Set compiler flags to treat warnings as errors
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libstdc++ -static-libgcc")


# Bass library test
add_executable(Bass_test
        bassTest.cpp
)

# Link the executable with the necessary libraries
target_link_libraries(Bass_test bass)
copy_bass_dll_to_target(Bass_test)


# PiAlarm display test
if(UNIX AND NOT APPLE)

    add_executable(Display_test
            displayTest.cpp
    )
    target_link_libraries(Display_test PRIVATE
            PiAlarm_hardware
            PiAlarm_gfx
    )
    # Copy assets directory to the target directory after build
    add_custom_command(TARGET Display_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Display_test>/assets)

endif()
