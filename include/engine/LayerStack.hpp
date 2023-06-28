#pragma once

#include "tugine/Core.hpp"
#include "Layer.hpp"

#include <vector>

class LayerStack : public Layer
{
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);
    void PopLayer(Layer *layer);
    void PopOverlay(Layer *overlay);

    std::vector<Layer *>::iterator begin() { return Layers.begin(); }
    std::vector<Layer *>::iterator end() { return Layers.end(); }

private:
    std::vector<Layer *> Layers;
    std::vector<Layer *>::iterator LayerInsert;
};