#include "Enemy.h"
#include "Game.h"
#include "Helpers/Sprite.h"
#include "Helpers/MathFuncs.h"
#include "Player.h"
#include "Tween.h"

const float Enemy::c_startingHealth = 1.0f;
const float Enemy::c_speed = 0.0f;
const float Enemy::c_animSpeed = 0.3f;

Enemy::Enemy(Game* pGame)
    : GameObject( pGame )
    , m_tweenSize( m_visibleScale )
    , m_health( c_startingHealth )
{
    m_radius = 20.0f;

    m_pSprites[0] = new Sprite( pGame->getTexture("Bat1") );
    m_pSprites[1] = new Sprite( pGame->getTexture("Bat2") );

    for( int i=0; i<2; i++ )
    {
        m_pSprites[i]->setOrigin( { 0.5f, 0.5f } );
    }

    setScale( { 4.0f, 4.0f } );

    m_animTimer = randFloat( 0.0f, c_animSpeed );
}

Enemy::~Enemy()
{
    delete m_pSprites[0];
    delete m_pSprites[1];
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
}

void Enemy::draw(bool drawDebugData)
{
    m_pSprites[m_currentFrame]->draw( m_position, m_angle, m_scale * m_visibleScale );
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
    m_health = 0;
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
