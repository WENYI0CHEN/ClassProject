#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "GameObject.h"
#include "Tween.h"

class Sprite;

class Enemy : public GameObject
{
private:
    static const float c_startingHealth;
    static const float c_speed;
    static const float c_animSpeed;

public:
    Enemy(Game* pGame);
    virtual ~Enemy();

    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;

    void startDeathAnim();
    bool isFadingOut() { return m_fadingOut; }

    // Getters.
    float getHealth() { return m_health; }

    // Setters.
    void applyDamage(float damage);
    void setHealth(float health) { m_health = health; }
    void setActive(bool active) override;

private:
    float m_health = 0.0f;

    Sprite* m_pSprites[2] = {};
    float m_animTimer = 0;
    int m_currentFrame = 0;

    bool m_fadingOut = false;

    float m_visibleScale = 0.0f;

    Tween m_tweenSize;
};
