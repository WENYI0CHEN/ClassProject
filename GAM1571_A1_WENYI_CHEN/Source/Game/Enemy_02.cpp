#include "Enemy_02.h"
#include "Game.h"
#include "Helpers/Sprite.h"
#include "Helpers/MathFuncs.h"
#include "Player.h"
#include "Tween.h"

const float Enemy_02::c_startingHealth = 50.0f;
const float Enemy_02::c_speed = 30.0f;
const float Enemy_02::c_animSpeed = 1.0f;
const float Enemy_02::c_animTimer=1.0f;

Enemy_02::Enemy_02(Game* pGame)
    : Enemy(pGame)
{
    m_radius = 20.0f;

    m_pSprites[0] = new Sprite(pGame->getTexture("Clown1"));
    m_pSprites[1] = new Sprite(pGame->getTexture("Clown2"));

    for (int i = 0; i < 2; i++)
    {
        m_pSprites[i]->setOrigin({ 0.5f, 0.5f });
    }

    setScale({ 2.0f, 2.0f });
    m_stopingAnimTimer = c_animTimer;
    m_animTimer = randFloat(0.0f, c_animSpeed);
}

Enemy_02::~Enemy_02()
{
    delete m_pSprites[0];
    delete m_pSprites[1];
}

void Enemy_02::update(float deltaTime)
{
    m_tweenSize.update(deltaTime);

    // Deactivate when tween is finished.
    if (m_fadingOut == true && m_tweenSize.isRunning() == false)
    {
        m_active = false;
        return;
    }

    // Move toward player.
    vec2 dir = m_pGame->getPlayer()->getPosition() - m_position;
    dir.normalize();

    if (m_walkingAnimTimer < c_animTimer)
    {
        m_walkingAnimTimer += deltaTime;
        vec2 velocity = dir * c_speed;
        m_position += velocity * deltaTime;
        // Update animation.
        m_animTimer += deltaTime;
        if (m_animTimer >= c_animSpeed)
        {
            m_animTimer -= c_animSpeed;
            m_currentFrame = 1 - m_currentFrame;

        }
    }

    if (m_walkingAnimTimer >= c_animTimer)
    {
        m_stopingAnimTimer -= deltaTime;
        m_currentFrame = 0;
        if (m_stopingAnimTimer <= 0)
        {
            m_walkingAnimTimer = 0;
            m_stopingAnimTimer = c_animTimer;
        }
    }

}

void Enemy_02::draw(bool drawDebugData)
{
    m_pSprites[m_currentFrame]->draw(m_position, m_angle, m_scale * m_visibleScale);
}

void Enemy_02::setActive(bool active)
{
    Enemy::setActive(active);

    if (active)
    {
        setHealth(c_startingHealth);
        m_walkingAnimTimer = 0.0f;
        m_stopingAnimTimer = c_animTimer;

    }
}