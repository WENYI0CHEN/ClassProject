#include "Bullet.h"
#include "Helpers/MathFuncs.h"

const Color Bullet::c_color = GREEN;
const float Bullet::c_speed = 100.0f;

Bullet::Bullet(Game* pGame)
    : GameObject( pGame )
{
    m_radius = 10.0f;
    m_pSprite = new Sprite(pGame->getTexture("Bullet_01"));
}

Bullet::~Bullet()
{
    delete m_pSprite;
}

void Bullet::update(float deltaTime)
{
    m_position += m_direction.getNormalized() * c_speed * deltaTime;
    // TODO: If the bullet goes off screen, deactivate it.
    if (m_position.x > GetScreenWidth() || m_position.y > GetScreenHeight() || m_position.x < 0 || m_position.y<0)
        setActive(false);


}

void Bullet::setActive(bool active)
{
    m_active = active;
}

void Bullet::draw(bool drawDebugData)
{
    m_pSprite->draw({ m_position.x,m_position.y }, 0.0f, vec2(0.2f, 0.2f));
}

void Bullet::reset()
{
    m_active = false;
}