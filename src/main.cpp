#include "Engine/Engine.hpp"
#include "debug/Log.hpp"
#include "Events/ApplicationEvent.hpp"
int main() {
    Engine engine;
    Log::Init();
    TUG_CORE_ERROR("Tugine Engine");
    TUG_INFO("Engine client");

    WindowResizeEvent e(1280, 720);
    TUG_TRACE(e);
    engine.Initialize();
    engine.MainLoop();

    return 0;
}