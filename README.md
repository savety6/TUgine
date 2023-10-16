# TUgine

## What is this?

CMake based C++ Project with support for glfw, glad, imgui, spdlog, tracy, stbi, cgltf

This project is split into three parts

`Project` and `TUgine` and `lib`

`Project` will be your actual project/game,
`TUgine` containing graphics/game engine abstractions.

`lib` contains the list of dependencies used by projects across the solution, in this case, `imgui`, `spdlog`, `glm`, `tracy`, `stbi` and `cgltf`.

<!-- ![Screenshot](screenshots/screenshot.png) -->

## What are the goals of this project?

- 2D/3D graphics support
- Cross platform (Windows/Mac/Linux)
- Physics, Audio, user input, networking, scripting, etc. support

## What do you need?

- `CMake`
- for VSCode the `CMake Tools` extension from Microsoft
- a compiler of your choice (Clang, GCC, MSVC)

Make sure to run `git submodule update --init --recursive` 
