#pragma once

#include "Event/Event.h"
#include "GLFW/glfw3.h"
#include "TUgine/Core.hpp"

namespace TUgine
{

    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string &title = "Hazel Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event &)>;

        Window(const WindowProps &props = WindowProps());
        virtual ~Window();

        void OnUpdate();

        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

        void SetEventCallback(const EventCallbackFn &callback);
        void SetVSync(bool enabled);
        bool IsVSync() const;

        static Window *Create(const WindowProps &props = WindowProps());
    private:
        virtual void Init(const WindowProps &props);
        virtual void Shutdown();
    private:
        GLFWwindow *m_Window;
        struct WindowData{
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };

}