#pragma once
#include "raylib.h"
#include "Helpers/Vector.h"
#include "Weapon.h"
#include<vector>
#include "Tween.h"



class Weapon_Gun_02 : public Weapon
{
private:
    static const float c_weaponFreezeTimer;
    static const int c_initialWeaponDamage;
    static const int c_initialWeaponRadius;
    static const int c_initialWeaponAmount;
    static const int c_weaponDamageIncreasePerLevel;
   
    static const float c_weaponRadiusIncreasePerLevel;

public:
    Weapon_Gun_02(Game* pGame);
    virtual ~Weapon_Gun_02();

    virtual void reset() override;
    virtual void update(float deltaTime) override;
    virtual void draw(bool drawDebugData) override;
    virtual void handleCollisions(EnemyList& enemyList) override;
    virtual void levelUp() override;
    void startDeathAnim();
    void setActive(float deltaTime);
    void setAngle(float angle) { m_angle = angle; }

private:
    Sprite* m_pSprite;
    vec2 m_weaponPos = { 0,0 };
    float m_angle = 0;
    float m_weaponRadius = 0;
    int m_weaponDamage = 0;
    float m_currentWeaponSpawnTime = 0.0f;
    bool m_fadingOut = false;
    Tween m_tweenSize;
    float m_visibleScale = 1.0f;
    int m_currentWeaponAmount = 0;
};