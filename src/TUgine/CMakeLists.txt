cmake_minimum_required(VERSION 3.14)
project(TUgine)

add_subdirectory(lib)

file(GLOB sourceFiles
    "*.cpp"
)

add_library(TUgine ${sourceFiles})

find_package(OpenGL REQUIRED)

target_include_directories(TUgine PUBLIC include)

target_link_libraries(TUgine PRIVATE glfw glad glm TracyClient spdlog imgui)