#include "Engine/Engine.hpp"

int main() {
    Engine engine;

    engine.Initialize();
    engine.MainLoop();

    return 0;
}