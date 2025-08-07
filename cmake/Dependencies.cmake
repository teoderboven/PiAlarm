cmake_minimum_required(VERSION 3.27)

set(BUILD_SHARED_LIBS OFF)


include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.15.3
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG        1.11.3
)
set(CURL_USE_LIBPSL OFF)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG        VER-2-13-3
)
set(FT_DISABLE_HARFBUZZ ON)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(
        utf8cpp
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG        v3.2.5
)
FetchContent_MakeAvailable(utf8cpp)
