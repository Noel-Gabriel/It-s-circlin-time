cmake_minimum_required(VERSION 3.15...3.30)

project(CirclinTime VERSION 1.0
                    DESCRIPTION "It's circlin time"
                    LANGUAGES C)

add_compile_options(-Wall -I${PROJECT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(CirclinTime)

include(FetchContent)


FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

target_link_libraries(CirclinTime PRIVATE raylib)

add_subdirectory(src)
add_subdirectory(test)
