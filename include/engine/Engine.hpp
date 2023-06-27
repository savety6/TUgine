#pragma once

#define GLFW_INCLUDE_NONE // GLFW then includes only the barebones OpenGL headers

#include "debug/Log.hpp"
#include "Tugine/Window.hpp" 
#include "Events/ApplicationEvent.hpp"
#include "Engine/LayerStack.hpp"
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

    void PushLayer( Layer* layer );
    void PushOverlay( Layer* layer ); 
private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> window; 
    bool running = true; 

    LayerStack layerStack;
};