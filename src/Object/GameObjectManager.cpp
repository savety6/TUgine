#include "Object/GameObjectManager.hpp"

GameObjectManager::GameObjectManager() {
    // Constructor logic here
}

GameObjectManager::~GameObjectManager() {
    for (auto object : objects) {
        delete object;
    }
    objects.clear();
}

void GameObjectManager::UpdateAll(float dt) {
    for (auto object : objects) {
        object->Update(dt);
    }
}

void GameObjectManager::RenderAll() {
    for (auto object : objects) {
        object->Render();
    }
}

void GameObjectManager::Add(GameObject* object) {
    objects.push_back(object);
}