#pragma once

#define GLFW_INCLUDE_NONE // GLFW then includes only the barebones OpenGL headers
#include <GLFW/glfw3.h>

class Engine {
public:
    Engine();
    ~Engine();

    void Initialize(); // Initialize the engine
    void Run(); // Main game loop
    void MainLoop(); 
    void Render(); 
private:
    GLFWwindow* window; // Add this


};