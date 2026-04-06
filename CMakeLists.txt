cmake_minimum_required(VERSION 3.7.2)

project(HaruhiEngine)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(SDL3 REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/engine
)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    engine/*.c
)

add_library(HaruhiEngine STATIC
    ${ENGINE_SOURCES}
)

add_executable(Haruhi
    Engine/Haruhi.c
)

target_link_libraries(HaruhiEngine
    PUBLIC
    glfw
    SDL3::SDL3
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Haruhi PRIVATE DEBUG)
endif()

set_target_properties(Haruhi PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)
