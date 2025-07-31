cmake_minimum_required(VERSION 3.10)

add_library(bass STATIC IMPORTED GLOBAL)

# Headers
set_target_properties(bass PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(WIN32)
    if(MINGW)
        set(BASS_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows/bass.a)
    else() # MSVC
        set(BASS_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows/bass.lib)
    endif()

    set(BASS_DLL ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows/bass.dll)

    set_target_properties(bass PROPERTIES
            IMPORTED_LOCATION "${BASS_DLL}"           # Runtime
            IMPORTED_IMPLIB "${BASS_BIN_PATH}"        # Linking
    )

elseif(UNIX AND NOT APPLE) # Linux
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(BASS_SO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/x86_64/libbass.so)

    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686" OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86")
        set(BASS_SO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/x86/libbass.so)

    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(BASS_SO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/aarch64/libbass.so)

    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        set(BASS_SO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux/armhf/libbass.so)

    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()

    set_target_properties(bass PROPERTIES
            IMPORTED_LOCATION "${BASS_SO_PATH}"
    )
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Copy the BASS runtime library to the output directory after building targets that link against it
# This is necessary for the application to run correctly, as it needs the BASS library at runtime.
# The function `copy_bass_dll_to_target` can be called for each target that links against `bass`.
function(copy_bass_dll_to_target target)
    get_target_property(BASS_RUNTIME_PATH bass IMPORTED_LOCATION)

    if(EXISTS "${BASS_RUNTIME_PATH}")
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${BASS_RUNTIME_PATH}"
                $<TARGET_FILE_DIR:${target}>
        )
    else()
        message(WARNING "IMPORTED_LOCATION for `bass` is not set or does not exist: ${BASS_RUNTIME_PATH}")
    endif()
endfunction()

