#pragma once

#include "engine/Layer.hpp"
#include "events/Event.hpp"

class Editor: public Layer  
{
public:
    Editor();
    ~Editor();

    void OnAttach();
    void OnDetach();

    void OnUpdate();
    void OnEvent(Event& event);

private:
    float lastFrameTime = 0.0f;
};
