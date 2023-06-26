#include "tugine/Platform/WindowsWindow.hpp"
#include "debug/Log.hpp"
#include "tugine/Core.hpp"

static bool s_GLFWInitialized = false;

Window* Window::Create(const WindowProps& props)
{
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props)
{
    data.Title = props.Title;
    data.Width = props.Width;
    data.Height = props.Height;

    TUG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        TUG_CORE_ASSERT(success, "Could not initialize GLFW!");

        s_GLFWInitialized = true;
    }

    window = glfwCreateWindow((int)props.Width, (int)props.Height, data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &data);
    SetVSync(true);

}
void WindowsWindow::Shutdown()
{
    glfwDestroyWindow(window);
}

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void WindowsWindow::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
    return data.VSync;
}

 