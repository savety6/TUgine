#pragma once

#include "engine/Layer.hpp"
#include "events/Event.hpp"

class Engine: public Layer  
{
public:
    Engine();
    ~Engine();

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate() override;
    void OnEvent(Event& event) override;

private:

};
