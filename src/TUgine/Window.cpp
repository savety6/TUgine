#include "TUgine/Window.hpp"
#include "TUgine/Core.hpp"

namespace TUgine
{
    static bool s_GLFWInitialized = false;

    Window *Window::Create(const WindowProps &props)
    {
        return new Window(props);
    }

    Window::Window(const WindowProps &props)
    {
        Init(props);
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    unsigned int Window::GetWidth() const
    {
        return m_Data.Width;
    }

    unsigned int Window::GetHeight() const
    {
        return m_Data.Height;
    }

    void Window::SetEventCallback(const EventCallbackFn &callback)
    {
        m_Data.EventCallback = callback;
    }

    void Window::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    void Window::Init(const WindowProps &props)
    {
        m_Data = WindowData();
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        TUG_CORE_INFO("Created window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            TUG_CORE_ASSERT(success, "Could not initialize GLFW!");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

} // namespace TUgine
