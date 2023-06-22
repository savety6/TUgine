#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <vector>
#include "GameObject.hpp"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();

    void UpdateAll(float dt);
    void RenderAll();

    void Add(GameObject* object);

private:
    std::vector<GameObject*> objects;
};

#endif // GAME_OBJECT_MANAGER_H