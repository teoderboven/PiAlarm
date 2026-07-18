
set(LIB_NAME PiAlarm_utils)

add_library(${LIB_NAME} INTERFACE)

set(HEADERS
        argsUtils.hpp
        consoleDisplayUtils.hpp
        consoleUtils.hpp
        ViewFormatUtils.hpp
        WeatherUtils.hpp
)

target_sources(${LIB_NAME} INTERFACE
        ${HEADERS}
)

target_link_libraries(${LIB_NAME} INTERFACE
        PiAlarm_common
)

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
