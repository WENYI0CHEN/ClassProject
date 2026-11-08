#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "GameObject.h"
#include "Helpers/Sprite.h"
#include"Game.h"

class Bullet : public GameObject
{
private:
    static const Color c_color;
    static const float c_speed;

public:
    Bullet(Game* pGame);
    virtual ~Bullet();
    virtual void setActive(bool active)override;
    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData)override;
    virtual void reset()override;

    // Getters.

    // Setters.
    void setDirection(vec2 dir) { m_direction = dir; }

private:
    vec2 m_direction = 0;
    Sprite* m_pSprite;
};
