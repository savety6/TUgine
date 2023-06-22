#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
public:
    GameObject();
    virtual ~GameObject();

    virtual void Update(float dt);
    virtual void Render();
    void SetPosition(float x, float y) {
        position[0] = x;
        position[1] = y;
    }
    // getters
    float GetPositionX() { return position[0]; }
    float GetPositionY() { return position[1]; }

private:
    float position[2]; // X and Y position of the object
    float color[3]; // RGB color of the object
};

#endif // GAME_OBJECT_H