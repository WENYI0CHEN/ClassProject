#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "GameObject.h"
#include "Tween.h"
#include"Enemy.h"

class Sprite;

class Enemy_02 : public Enemy
{
private:
    static const float c_startingHealth;
    static const float c_speed;
    static const float c_animSpeed;
    static const float c_animTimer;
public:
    Enemy_02(Game* pGame);
    virtual ~Enemy_02();

    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;
    void setActive(bool active) override;

private:
    float m_health = 0.0f;

    Sprite* m_pSprites[2] = {};
    float m_animTimer = 0;
    int m_currentFrame = 0;
    float m_walkingAnimTimer = 0;
    float m_stopingAnimTimer = 0;
};
