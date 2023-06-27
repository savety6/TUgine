
#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Window.hpp"

class MacWindow : public Window
{
public:
    MacWindow(const WindowProps &props);
    virtual ~MacWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return data.Width; }
    inline unsigned int GetHeight() const override { return data.Height; }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn &callback) override { data.EventCallback = callback; }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();

private:
    GLFWwindow *window;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData data;
};
