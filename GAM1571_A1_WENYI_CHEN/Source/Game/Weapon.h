#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include"Types.h"
#include "Helpers/Sprite.h"

class Enemy;
class Game;

class Weapon
{
public:
    Weapon(Game* pGame);
    virtual ~Weapon();

    virtual void reset() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(bool drawDebugData) = 0;
    virtual void handleCollisions(EnemyList& enemyList) = 0;
    virtual void levelUp() = 0;
    bool isActive() { return m_isActive; }

protected:
    Game* m_pGame;
    bool m_isActive = true;
};
