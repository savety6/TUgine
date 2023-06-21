cmake_minimum_required(VERSION 3.0.0)
project(tugine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include_directories(include)
include_directories(include/engine)

file(GLOB_RECURSE SOURCES "src/*.cpp")
# add all files in src/ to the executable
add_executable(tugine ${SOURCES})


# add_executable(tugine src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
