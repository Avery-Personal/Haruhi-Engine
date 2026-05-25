cmake_minimum_required(VERSION 3.7.2)

project(HaruhiEngine)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(SDL3 REQUIRED)

add_subdirectory(Engine/Third-Party/DiligentEngine)

file(GLOB_RECURSE ALL_ENGINE_SOURCES CONFIGURE_DEPENDS
    Engine/*.c
)

list(FILTER ALL_ENGINE_SOURCES EXCLUDE REGEX ".*/Platform/OperatingSystem/.*")
list(FILTER ALL_ENGINE_SOURCES EXCLUDE REGEX ".*/Third-Party/.*")

if(WIN32)
    set(PLATFORM_SOURCE
        Engine/Platform/OperatingSystem/Windows/PlatformWin.c
    )
elseif(APPLE)
    set(PLATFORM_SOURCE
        Engine/Platform/OperatingSystem/MacOS/PlatformMacOS.c
    )
elseif(UNIX)
    set(PLATFORM_SOURCE
        Engine/Platform/OperatingSystem/Linux/PlatformLinux.c
    )
endif()

add_library(HaruhiEngine STATIC
    ${ALL_ENGINE_SOURCES}
    ${PLATFORM_SOURCE}
)

add_executable(Haruhi
    Engine/Haruhi.c
)

target_include_directories(HaruhiEngine
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(HaruhiEngine
    PUBLIC
    glfw
    SDL3::SDL3

    Diligent-BuildSettings
    Diligent-Common
    Diligent-GraphicsAccessories
    Diligent-GraphicsEngineOpenGL-static
)

target_link_libraries(Haruhi
    PRIVATE
    HaruhiEngine
)

target_compile_definitions(Haruhi
    PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(Haruhi PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)
