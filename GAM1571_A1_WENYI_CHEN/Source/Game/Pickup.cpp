#include "Pickup.h"
#include "Game.h"
#include "Player.h"

const float Pickup::c_size = 10.0f; // Pixels.
const Color Pickup::c_color = BLUE;

Pickup::Pickup(Game* pGame)
    : GameObject( pGame )
    , m_tweenPosX( m_position.x )
    , m_tweenPosY( m_position.y )
{
    m_active = false;

    m_position.set( 0, 0 );
    m_radius = c_size;
}

Pickup::~Pickup()
{
}

void Pickup::update(float deltaTime)
{
    m_tweenPosX.changeTarget( m_pGame->getPlayer()->getPosition().x );
    m_tweenPosY.changeTarget( m_pGame->getPlayer()->getPosition().y );

    m_tweenPosX.update( deltaTime );
    m_tweenPosY.update( deltaTime );

    if( m_animating && m_tweenPosX.isRunning() == false && m_tweenPosY.isRunning() == false )
    {
        m_animating = false;
        setActive( false );
    }
}

void Pickup::draw(bool drawDebugData)
{
    DrawCircle( m_position.x, m_position.y, m_radius, BLACK );
    DrawCircle( m_position.x, m_position.y, m_radius-2, c_color );
}

void Pickup::pickupObject(vec2 destPos)
{
    m_animating = true;

    m_tweenPosX.start( destPos.x, 0.5f, 0.0f, easingInBack );
    m_tweenPosY.start( destPos.y, 0.5f, 0.0f, easingInBack );
}
