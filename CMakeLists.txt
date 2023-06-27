# initial cmake file for tugine
cmake_minimum_required(VERSION 3.0.0)
project(tugine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

# testing
include(CTest)
enable_testing()

# glfw and glew
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

# spdlog
add_subdirectory(libraries/spdlog)
list(APPEND CMAKE_PREFIX_PATH "./libraries/spdlog/build")
find_package(spdlog REQUIRED)

# include directories for header files
include_directories(include)
# include subdirectories for header files
include_directories(include/tugine)
include_directories(include/engine)
include_directories(include/events)

# ImGui
include_directories(libraries/imgui)
include_directories(libraries/imgui/backends) # Contains the implementation for GLFW and OpenGL

# add all files in src/ to SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")

# add ImGui files
file(GLOB IMGUI_SOURCES "libraries/imgui/*.cpp" "libraries/imgui/*.h" "libraries/imgui/backends/imgui_impl_glfw.cpp" "libraries/imgui/backends/imgui_impl_opengl3.cpp")
list(APPEND SOURCES ${IMGUI_SOURCES})

# add all files in src/ to the executable
add_executable(tugine ${SOURCES})

# link libraries
target_link_libraries(tugine spdlog)
target_link_libraries(tugine glfw)
target_link_libraries(tugine GLEW::GLEW)

# no clue what this does
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
