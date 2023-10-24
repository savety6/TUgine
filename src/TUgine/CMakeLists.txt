cmake_minimum_required(VERSION 3.14)
project(TUgine)

add_subdirectory(lib)

file(GLOB sourceFiles
    "*.cpp"
)

file(GLOB headerFiles
    "**.h"
    "**.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Event/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/TUgine/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/TUgine/*.hpp"
)
add_library(TUgine ${sourceFiles} ${headerFiles})

find_package(OpenGL REQUIRED)

target_include_directories(TUgine PUBLIC include)

target_link_libraries(TUgine PRIVATE glfw glad glm TracyClient spdlog imgui)