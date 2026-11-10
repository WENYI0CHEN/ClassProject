#pragma once

#include "Types.h"

class Enemy;
class Game;

class Weapon
{
public:
    Weapon(Game* pGame);
    virtual ~Weapon();

    virtual void reset() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(fw::Camera* pCamera) = 0;
    virtual void handleCollisions(EnemyList& enemyList) = 0;
    virtual void levelUp() = 0;

protected:
    Game* m_pGame;
};
