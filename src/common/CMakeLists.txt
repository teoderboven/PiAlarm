
set(LIB_NAME PiAlarm_common)

add_library(${LIB_NAME} INTERFACE)

set(SOURCES
        Observable.hpp
        Observer.h
        WeatherCondition.h
)

target_sources(${LIB_NAME} INTERFACE
        ${SOURCES}
)

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
