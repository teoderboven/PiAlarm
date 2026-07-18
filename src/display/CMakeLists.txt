
set(LIB_NAME PiAlarm_display)

add_library(${LIB_NAME} INTERFACE)

set(HEADERS
        ViewOutputConfig.h
)

target_sources(${LIB_NAME} INTERFACE
        ${HEADERS}
)
if(DISPLAY_SSD1322)
    target_link_libraries(${LIB_NAME} INTERFACE
            PiAlarm_hardware
            PiAlarm_gfx
    )
endif()

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
