cmake_minimum_required(VERSION 3.0.0)
project(tugine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

# spdlog
add_subdirectory(libraries/spdlog)
list(APPEND CMAKE_PREFIX_PATH "./libraries/spdlog/build")
find_package(spdlog REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
# add all files in src/ to the executable
add_executable(tugine ${SOURCES})
target_link_libraries(tugine spdlog)

target_link_libraries(tugine glfw)
target_link_libraries(tugine GLEW::GLEW)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
