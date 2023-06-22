#include "Object/GameObject.hpp"
#include <GL/glew.h> 

GameObject::GameObject() {
    position[0] = 0.0f;
    position[1] = 0.0f;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
}

GameObject::~GameObject() {
    // Destructor logic here
}

void GameObject::Update(float dt) {
    // Update logic here
}

void GameObject::Render() {
    // Set the object color
    glColor3f(color[0], color[1], color[2]);

    // Draw a square for the object
    glBegin(GL_QUADS);
        glVertex2f(position[0] - 0.1f, position[1] - 0.1f);
        glVertex2f(position[0] + 0.1f, position[1] - 0.1f);
        glVertex2f(position[0] + 0.1f, position[1] + 0.1f);
        glVertex2f(position[0] - 0.1f, position[1] + 0.1f);
    glEnd();
}