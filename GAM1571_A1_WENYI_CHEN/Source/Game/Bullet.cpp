#include "Bullet.h"
#include "Helpers/MathFuncs.h"

const Color Bullet::c_color = GREEN;
const float Bullet::c_speed = 5500.0f;

Bullet::Bullet(Game* pGame)
    : GameObject( pGame )
{
    m_radius = 10.0f;
}

Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
    // TODO: If the bullet goes off screen, deactivate it.
}
