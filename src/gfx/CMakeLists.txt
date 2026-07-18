cmake_minimum_required(VERSION 3.27)

project(PiAlarm_gfx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
        Bitmap.h
        Canvas.h
        Glyph.h
        IBuffer.h
        IFont.h
        Pictogram.h
        SDD1322Buffer.h
        TrueTypeFont.h
        TrueTypeFontCache.h
        Types.h
)

set(SOURCES
        Canvas.cpp
        Pictogram.cpp
        SDD1322Buffer.cpp
        TrueTypeFont.cpp
        TrueTypeFontCache.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${PROJECT_NAME} PUBLIC
        freetype
        utf8cpp
        lodepng
)
