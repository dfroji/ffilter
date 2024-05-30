cmake_minimum_required(VERSION 3.8)

project(ffilter
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(fparser
    GIT_REPOSITORY https://gitlab.com/froji/fparser
    GIT_TAG origin/main
    )

FetchContent_MakeAvailable(fparser)

include_directories(fparser
    ${fparser_SOURCE_DIR}/include
    )

set(lodepng ${CMAKE_BINARY_DIR}/external/lodepng)
file(DOWNLOAD
    https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp
    ${lodepng}/lodepng.cpp
    )
file(DOWNLOAD
    https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.h
    ${lodepng}/lodepng.h
    )

include_directories(lodepng
    ${lodepng}/
    )

add_executable(ffilter
    src/main.cpp
    ${lodepng}/lodepng.cpp
    )

target_link_libraries(ffilter
    PRIVATE fparser
    )
