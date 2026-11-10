#pragma once

#include "GameObject.h"

class Sprite;

class Player : public GameObject
{
private:
    static const float c_speed; // Pixels per second.
    static const float c_pickupRadius; // Pixels.
    static const float c_startingHealth;

public:
    Player(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~Player();

    virtual void reset() override;
    virtual void update(float deltaTime) override;
    virtual void draw(fw::Camera* pCamera) override;

    // Input event methods.
    void onKey(int keyCode, bool pressed);
    //void onMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
    //void onMouseMovedEvent(float mouseX, float mouseY);

    void handleCollision(vec2 collisionPoint, vec2 collisionNormal);

    // Getters.
    float getHealth() { return m_health; }
    float getPickupRadius() { return m_pickupRadius; }

    void applyDamage(float damage);

private:
    float m_maxHealth = c_startingHealth;
    float m_health = 0;
    float m_pickupRadius = 0;

    vec2 m_controls = { 0, 0 };
};
