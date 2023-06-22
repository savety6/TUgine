#include "Engine/Engine.hpp"
#include "debug/Log.hpp"
int main() {
    Engine engine;
    Log::Init();
    TUG_CORE_ERROR("Tugine Engine");
    TUG_INFO("Engine client");
    engine.Initialize();
    engine.MainLoop();

    return 0;
}