#pragma once

#include "tugine/Core.hpp"
#include "events/Event.hpp"

#include <string>

class  Layer
{
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnEvent(Event &event) {}

    inline const std::string &GetName() const { return DebugName; }

protected:
    std::string DebugName;
};
