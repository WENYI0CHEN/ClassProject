#include "Framework.h"
#include "Enemy.h"
#include "Game.h"
#include "Helpers/MathFuncs.h"
#include "Player.h"
#include "Tween.h"

const float Enemy::c_startingHealth = 50.0f;
const float Enemy::c_speed = 30.0f;
const float Enemy::c_animSpeed = 0.3f;

Enemy::Enemy(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : GameObject( pGame, pMesh, pShader )
    , m_tweenSize( m_visibleScale )
    , m_health( c_startingHealth )
{
    m_color = color4f::red();
    m_radius = 20.0f;

    setScale( { 40.0f, 40.0f } );

    m_animTimer = randFloat( 0.0f, c_animSpeed );
}

Enemy::~Enemy()
{
}

void Enemy::update(float deltaTime)
{
    m_tweenSize.update( deltaTime );

    // Deactivate when tween is finished.
    if( m_fadingOut == true && m_tweenSize.isRunning() == false )
    {
        m_active = false;
        return;
    }

    // Move toward player.
    vec2 dir = m_pGame->getPlayer()->getPosition() - m_position;
    dir.normalize();

    vec2 velocity = dir * c_speed;
    m_position += velocity * deltaTime;

    // Update animation.
    m_animTimer += deltaTime;
    if( m_animTimer > c_animSpeed )
    {
        m_animTimer = 0.0f;

        m_currentFrame++;
        if( m_currentFrame > 1 )
            m_currentFrame = 0;
    }
}

void Enemy::draw(fw::Camera* pCamera)
{
    //GameObject::draw( pCamera );
    m_pMesh->draw( pCamera, m_pShader, m_scale * m_visibleScale, m_angle, m_position, m_color, m_pTexture );
}

void Enemy::startDeathAnim()
{
    m_health = 0.0f;
    m_visibleScale = 1.0f;
    m_fadingOut = true;

    m_tweenSize.start( 0.0f, 0.5f, 0.0f, easingLinear );
}

void Enemy::applyDamage(float damage)
{
    m_health -= damage;
    if( m_health < 0 )
    {
        startDeathAnim();
    }
}

void Enemy::setActive(bool active)
{
    GameObject::setActive( active );

    m_visibleScale = 0.0f;
    m_fadingOut = false;
    m_health = c_startingHealth;

    if( m_active == true )
    {
        m_tweenSize.start( 1.0f, 1.5f, 0.0f, easingOutElastic );
    }
}
