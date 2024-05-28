cmake_minimum_required(VERSION 3.8)

project(ffilter
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(fparser
    GIT_REPOSITORY https://gitlab.com/froji/fparser
    GIT_TAG 06a0378a363d8a6dec0378878edf09b8de94824b
    )

include_directories(fparser
    ${fparser_SOURCE_DIR}/include
    )

FetchContent_MakeAvailable(fparser)

add_executable(ffilter
    src/main.cpp
    )

target_link_libraries(ffilter
    PRIVATE fparser
    )
