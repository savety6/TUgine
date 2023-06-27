#pragma once

#define GLFW_INCLUDE_NONE // GLFW then includes only the barebones OpenGL headers
#include <GLFW/glfw3.h>
#include "Object/GameObject.hpp"
#include "Object/GameObjectManager.hpp"
#include "Tugine/Window.hpp" 
#include "Events/ApplicationEvent.hpp"
#include <memory>
class Engine {
public:
    Engine();
    ~Engine();

    void Initialize(); // Initialize the engine
    void Run(); // Main game loop
    void MainLoop(); 
    void Render(); 
    void OnEvent(Event& e);
private:
    bool OnWindowClose(WindowCloseEvent& e);


    std::unique_ptr<Window> window; 
    bool running = true; 
    // GLFWwindow* window; // Add this
    // GameObject* player;
    // GameObjectManager gameObjectManager;
};