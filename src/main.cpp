#include "Engine/Engine.hpp"
#include "debug/Log.hpp"
#include "Events/ApplicationEvent.hpp"
int main() {
    Engine engine;
    
    engine.Initialize();
    engine.MainLoop();
    

    return 0;
}