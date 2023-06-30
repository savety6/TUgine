cmake_minimum_required(VERSION 3.0.0)
project(tugine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()

add_subdirectory(libraries/glfw)

# add the glew submodule path to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/libraries/glew")

add_subdirectory(libraries/spdlog)
list(APPEND CMAKE_PREFIX_PATH "./libraries/spdlog/build")
find_package(spdlog REQUIRED)

include_directories(include)
include_directories(include/tugine)
include_directories(include/engine)
include_directories(include/tugine/Platform/OpenGL)

include_directories(libraries/glfw/include)

include_directories(libraries/imgui)
include_directories(libraries/imgui/backends) 

file(GLOB_RECURSE SOURCES "src/*.cpp")

# list(APPEND SOURCES "libraries/imgui/backends/imgui_impl_glfw.cpp")
# list(APPEND SOURCES "libraries/imgui/backends/imgui_impl_opengl3.cpp")

file(GLOB IMGUI_SOURCES "libraries/imgui/*.cpp")
list(APPEND SOURCES ${IMGUI_SOURCES})

add_executable(tugine ${SOURCES})

# find and link GLEW
find_package(GLEW REQUIRED)
target_link_libraries(tugine GLEW::GLEW)

target_link_libraries(tugine spdlog)
target_link_libraries(tugine glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
