#include <TUgine/Application.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

#include <iostream>
#include <string>

namespace TUgine
{

    void Application::Run()
    {
        FrameMarkStart("App Run");
        if (!Initialize())
        {
            return;
        }

        TUG_CORE_INFO("App: Initialized");

        if (!Load())
        {
            return;
        }

        TUG_CORE_INFO("App: Loaded");

        double previousTime = glfwGetTime();
        while (!glfwWindowShouldClose(_windowHandle))
        {
            double currentTime = glfwGetTime();
            float deltaTime = static_cast<float>(currentTime - previousTime);
            previousTime = currentTime;

            glfwPollEvents();
            Update(deltaTime);
            Render(deltaTime);
        }

        TUG_CORE_INFO("App: Unloading");

        Unload();

        TUG_CORE_INFO("App: Unloaded");
        FrameMarkEnd("App Run");
    }

    void Application::Close()
    {
        glfwSetWindowShouldClose(_windowHandle, 1);
    }

    bool Application::IsKeyPressed(int32_t key)
    {
        return glfwGetKey(_windowHandle, key) == GLFW_PRESS;
    }

    bool Application::Initialize()
    {
        Log::Init();
        if (!glfwInit())
        {
            TUG_CORE_ERROR("Glfw: Unable to initialize");
            return false;
        }

        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        // make the window apears in the center of the screen
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

        const auto primaryMonitor = glfwGetPrimaryMonitor();
        const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

        constexpr int windowWidth = 800;
        constexpr int windowHeight = 600;

        _windowHandle = glfwCreateWindow(windowWidth, windowHeight, "TUgine", nullptr, nullptr);
        if (_windowHandle == nullptr)
        {
            TUG_CORE_ERROR("Glfw: Unable to create window");
            glfwTerminate();
            return false;
        }

        const auto screenWidth = primaryMonitorVideoMode->width;
        const auto screenHeight = primaryMonitorVideoMode->height;
        glfwSetWindowPos(_windowHandle, screenWidth / 2 - windowWidth / 2, screenHeight / 2 - windowHeight / 2);

        glfwMakeContextCurrent(_windowHandle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        AfterCreatedUiContext();
        ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        TUG_CORE_INFO("Glfw: Initialized and window created, OpenGL version: {}", glGetString(GL_VERSION));

        return true;
    }
    // OpenGL setup
    bool Application::Load()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEPTH_TEST);
        glDebugMessageCallback([](GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar *message,
                                  const void *)
                               {
            // Convert GLenum parameters to strings
            std::string _source;
            std::string _type;
            std::string _severity;

            switch (source) {
                case GL_DEBUG_SOURCE_API:             _source = "API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   _source = "Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: _source = "Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     _source = "Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     _source = "Application"; break;
                case GL_DEBUG_SOURCE_OTHER:           _source = "Other"; break;
            }

            switch (type) {
                case GL_DEBUG_TYPE_ERROR:               _type = "Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: _type = "Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  _type = "Undefined Behaviour"; break;
                case GL_DEBUG_TYPE_PORTABILITY:         _type = "Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         _type = "Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              _type = "Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          _type = "Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           _type = "Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER:               _type = "Other"; break;
            }

            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:         _severity = "high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       _severity = "medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          _severity = "low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: _severity = "notification"; break;
            }

            TUG_CORE_ERROR("GL CALLBACK: source = {}, type = {}, id = {}, severity = {}, message = {}\n", _source, _type, id, _severity, message); },
                               nullptr);

        glClearColor(0.05f, 0.02f, 0.07f, 1.0f);

        glfwSwapInterval(1);

        return true;
    }

    void Application::Unload()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        BeforeDestroyUiContext();
        ImGui::DestroyContext();

        glfwTerminate();
    }

    void Application::Render(float dt)
    {
        ZoneScopedC(tracy::Color::Red2);

        RenderScene(dt);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            RenderUI(dt);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui::EndFrame();
        }

        glfwSwapBuffers(_windowHandle);
    }

    void Application::RenderScene([[maybe_unused]] float dt)
    {
    }

    void Application::RenderUI([[maybe_unused]] float dt)
    {
    }

    void Application::Update([[maybe_unused]] float dt)
    {
    }

    void Application::AfterCreatedUiContext()
    {
    }

    void Application::BeforeDestroyUiContext()
    {
    }

}
