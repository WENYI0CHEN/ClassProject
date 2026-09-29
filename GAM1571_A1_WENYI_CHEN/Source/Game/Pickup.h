#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "Tween.h"

class Pickup : public GameObject
{
private:
    static const float c_size; // Pixels.
    static const Color c_color;

public:
    Pickup(Game* pGame);
    virtual ~Pickup();

    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;

    void pickupObject(vec2 destPos);
    bool isAnimating() { return m_animating; }

    // Getters.

    // Setters.

private:
    bool m_animating = false;

    Tween m_tweenPosX;
    Tween m_tweenPosY;
};
