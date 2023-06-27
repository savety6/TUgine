#include "Engine/Engine.hpp"
#include "Object/GameObjectManager.hpp"

#include <GL/glew.h> // GLEW includes
#include <GLFW/glfw3.h> // GLFW includes

#include "debug/Log.hpp"

#define BIND_EVENT_FN(x) std::bind(&Engine::x, this, std::placeholders::_1)

Engine::Engine() : window(nullptr) {
    // Constructor
    
}

Engine::~Engine() {
    // Destructor
}

void Engine::Initialize() {
    Log::Init();
    window = std::unique_ptr<Window>(Window::Create());
    window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

void Engine::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    TUG_INFO("{0}", e);
}

bool Engine::OnWindowClose(WindowCloseEvent& e) {
    running = false;
    return true;
}

void Engine::Run() {
    
}

void Engine::MainLoop() {
    // Main game loop
    while (running) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set the clear color to black
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)
        window->OnUpdate();
    }
    //log when the game loop ends
    TUG_INFO("Engine stopped");
}


void Engine::Render() {
    
}





