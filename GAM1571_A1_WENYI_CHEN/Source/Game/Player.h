#pragma once

#include "raylib.h"

#include "GameObject.h"
#include "Helpers/InputTypes.h"

class Sprite;

class Player : public GameObject
{
private:
    static const float c_speed; // Pixels per second.
    static const float c_pickupRadius; // Pixels.
    static const float c_startingHealth;

public:
    Player(Game* pGame);
    virtual ~Player();

    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;

    // Input event methods.
    void onKey(int keyCode, KeyState keyState);
    //void onMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
    //void onMouseMovedEvent(float mouseX, float mouseY);

    void handleCollision(vec2 collisionPoint, vec2 collisionNormal);

    // Getters.
    float getHealth() { return m_health; }
    float getPickupRadius() { return m_pickupRadius; }

private:
    float m_health = 0;
    float m_pickupRadius = 0;

    Sprite* m_pSprite;
    vec2 m_controls = { 0, 0 };
};
