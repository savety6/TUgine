#include "Engine/Engine.hpp"
#include "Object/GameObjectManager.hpp"

#include <GL/glew.h> // GLEW includes
#include <GLFW/glfw3.h> // GLFW includes

Engine::Engine() : window(nullptr) {
    // Constructor

    player = new GameObject();
    gameObjectManager.Add(player);
}

Engine::~Engine() {
    // Destructor
}

void Engine::Initialize() {
    // Initialization logic here

    // Initialize GLFW
    if (!glfwInit()) {
        // Initialization failed
    }

    // Set the desired OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        // Window or OpenGL context creation failed
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        // Initialization failed
    }

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Engine::Run() {
    // Main game loop logic here
}

void Engine::MainLoop() {
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render
        Render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Update the game objects
        gameObjectManager.UpdateAll(0.0f); // Replace 0.0f with a real delta time calculation later

        // Input handling
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            player->SetPosition(player->GetPositionX(), player->GetPositionY() + 0.01f); // Adjust the number for different speed
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            player->SetPosition(player->GetPositionX(), player->GetPositionY() - 0.01f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player->SetPosition(player->GetPositionX() - 0.01f, player->GetPositionY());
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player->SetPosition(player->GetPositionX() + 0.01f, player->GetPositionY());
        }

        // Render game objects
        gameObjectManager.RenderAll();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    
    }

    glfwTerminate();
}


void Engine::Render() {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT);

    // Setup orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1); // Set up the projection so that the coordinates -1 to 1 in both x and y are visible

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw a triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(0.0f, 0.5f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.5f, -0.5f);
    glEnd();
}





