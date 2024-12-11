cmake_minimum_required(VERSION 3.25)
project(Search_Engine)

set(CMAKE_CXX_STANDARD 23)

add_executable(Search_Engine
        include/converterjson.h
        src/converterjson.cpp
        include/invertedIndex.h
        src/invertedIndex.cpp
        main.cpp
        include/searchServer.h
        src/searchServer.cpp
        src/searchServer.cpp src/searchServer.cpp src/searchServer.cpp)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_MakeAvailable(json)

target_link_libraries(Search_Engine PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

include(GoogleTest)

add_subdirectory(tests)