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


# include directori for header files
include_directories(include)
# include subdirectori for header files
include_directories(include/tugine)
include_directories(include/engine)


# add all files in src/ to SOURCES
file(GLOB_RECURSE SOURCES "src/*.cpp")

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
