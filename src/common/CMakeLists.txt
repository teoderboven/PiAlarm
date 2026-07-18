
set(LIB_NAME PiAlarm_common)

add_library(${LIB_NAME} INTERFACE)

set(HEADERS
        Observable.hpp
        Observer.h
        WeatherCondition.h
)

target_sources(${LIB_NAME} INTERFACE
        ${HEADERS}
)

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
