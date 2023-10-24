#include "TUgine/Window.hpp"

namespace TUgine
{
    
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
        }
    
        unsigned int Window::GetWidth() const
        {
            return m_Data->Width;
        }
    
        unsigned int Window::GetHeight() const
        {
            return m_Data->Height;
        }
    
        void Window::SetEventCallback(const EventCallbackFn &callback)
        {
            m_Data->EventCallback = callback;
        }
    
        void Window::SetVSync(bool enabled)
        {
            m_Data->VSync = enabled;
        }
    
        bool Window::IsVSync() const
        {
            return m_Data->VSync;
        }
    
        void Window::Init(const WindowProps &props)
        {
            m_Data = std::make_unique<WindowData>();
            m_Data->Title = props.Title;
            m_Data->Width = props.Width;
            m_Data->Height = props.Height;
    
            TUG_INFO("Created window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        }
    
        void Window::Shutdown()
        {
        }
    
} // namespace TUgine
