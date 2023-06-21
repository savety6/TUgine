#pragma once

class System {
public:
    System();
    virtual ~System();

    virtual void Update() = 0; // All systems should be able to update
};
