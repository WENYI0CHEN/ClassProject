#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "GameObject.h"

class Bullet : public GameObject
{
private:
    static const Color c_color;
    static const float c_speed;

public:
    Bullet(Game* pGame);
    virtual ~Bullet();

    virtual void update(float deltaTime) override;

    // Getters.

    // Setters.
    void setDirection(vec2 dir) { m_direction = dir; }

private:
    vec2 m_direction = 0;
};
