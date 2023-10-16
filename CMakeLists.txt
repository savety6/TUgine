# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Define the project name
project(Project)

# Set the C++ standard to version 20
set(CMAKE_CXX_STANDARD 20)

# Define an option to enable colored output during the build process
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

# If the option is enabled and the compiler is GNU or Clang, add the appropriate compiler flag for colored output
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

# Add subdirectories for building libraries and executables
add_subdirectory(lib)
add_subdirectory(src/TUgine)
add_subdirectory(src/Project)