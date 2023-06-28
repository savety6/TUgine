#include "engine/Engine.hpp"
#include "debug/Log.hpp"
#include "events/ApplicationEvent.hpp"
int main() {
    Engine engine;
    
    engine.Initialize();
    engine.MainLoop();
    

    return 0;
}