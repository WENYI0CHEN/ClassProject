#include "Framework.h"
#include "Bullet.h"
#include "Helpers/MathFuncs.h"

const color4f Bullet::c_color = color4f::green();
const float Bullet::c_speed = 500.0f;

Bullet::Bullet(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : GameObject( pGame, pMesh, pShader )
{
    m_color = c_color;
    m_radius = 10.0f;
}

Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
    vec2 velocity = m_direction * c_speed;
    m_position += velocity * deltaTime;

    // If the bullet goes off screen, deactivate it.
    if( m_position.x < -64 || m_position.x > GetScreenWidth() ||
        m_position.y < -64 || m_position.y > GetScreenHeight() )
    {
        m_active = false;
    }
}

void Bullet::draw(fw::Camera* pCamera)
{
    GameObject::draw( pCamera );

    //float angleRadians = (float)(m_angle * PI/180.0f);
    // TODO: Fix me!!!!
    //DrawCircle( m_position.x, m_position.y, m_radius, BLACK );
    //DrawCircle( m_position.x, m_position.y, m_radius-2, c_color );
}
