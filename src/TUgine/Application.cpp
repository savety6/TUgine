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

        const auto primaryMonitor = glfwGetPrimaryMonitor();
        const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);

        constexpr int windowWidth = 1200;
        constexpr int windowHeight = 900;

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
        AfterCreatedUiContext();
        ImGui_ImplGlfw_InitForOpenGL(_windowHandle, true);
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        return true;
    }
    // OpenGL setup
    bool Application::Load()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEPTH_TEST);
        glDebugMessageCallback([](GLenum source,
                                  GLenum type,
                                  GLuint,
                                  GLenum severity,
                                  GLsizei,
                                  const GLchar *message,
                                  const void *)
                               {
        if (type == GL_DEBUG_TYPE_ERROR)
        {
            TUG_CORE_ERROR("GL CALLBACK: type = {}, severity = error, message = {}\n", type, message);
        } },
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
