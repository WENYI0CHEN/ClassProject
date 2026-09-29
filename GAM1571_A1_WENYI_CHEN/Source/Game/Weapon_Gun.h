#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"
#include "Weapon.h"

class Bullet;

class Weapon_Gun : public Weapon
{
private:
    static const int c_numBullets;
    static const int c_bulletDamage;
    static const float c_initialBulletSpawnTime;
    static const float c_bulletSpawnTimeDecreasePerLevel;

public:
    Weapon_Gun(Game* pGame);
    virtual ~Weapon_Gun();

    virtual void reset() override;
    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;
    virtual void handleCollisions(EnemyList& enemyList) override;
    virtual void levelUp() override;

    void spawnBullet(vec2 pos);

private:
    std::vector<Bullet*> m_bullets;

    float m_currentBulletSpawnTime = 0.0f;
    float m_bulletSpawnTimer = 0.0f;
};
