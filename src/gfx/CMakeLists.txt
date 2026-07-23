cmake_minimum_required(VERSION 3.27)

project(PiAlarm_gfx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        Bitmap.h
        Canvas.cpp
        Canvas.h
        Glyph.h
        IBuffer.h
        IFont.h
        Pictogram.cpp
        Pictogram.h
        SDD1322Buffer.cpp
        SDD1322Buffer.h
        TrueTypeFont.cpp
        TrueTypeFont.h
        TrueTypeFontCache.cpp
        TrueTypeFontCache.h
        Types.h
)

add_library(${PROJECT_NAME} STATIC
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${PROJECT_NAME} PUBLIC
        freetype
        utf8cpp
        lodepng
)
