#pragma once
#include <cstdint>
#include "Event/Event.h"
struct GLFWwindow;

class Application
{
public:
    void Run();

protected:
    void Close();
    bool IsKeyPressed(int32_t key);
    
    double GetDeltaTime();

    virtual void AfterCreatedUiContext();
    virtual void BeforeDestroyUiContext();
    virtual bool Initialize();
    virtual bool Load();
    virtual void Unload();
    virtual void RenderScene(float deltaTime);
    virtual void RenderUI(float deltaTime);
    virtual void Update(float deltaTime);


private:
    GLFWwindow* _windowHandle = nullptr;
    void Render(float deltaTime);

};