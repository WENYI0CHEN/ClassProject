#include "Game.h"
#include "Helpers/Sprite.h"
#include "Player.h"

const float Player::c_speed = 150.0f; // Pixels per second.
const float Player::c_pickupRadius = 100.0f; // Pixels.
const float Player::c_startingHealth = 100.0f;

Player::Player(Game* pGame)
    : m_pickupRadius( c_pickupRadius )
    , m_health( c_startingHealth )
{
    m_active = true;

    m_radius = 20.0f;

    m_pSprite = new Sprite( pGame->getTexture("Player") );
    m_scale = 4.0f;
}

Player::~Player()
{
    delete m_pSprite;
}

void Player::update(float deltaTime)
{
    vec2 dir = m_controls.getNormalized();

    // Lock to edges.
    if( m_position.x < 32.0f )
        m_position.x = 32.0f;
    if( m_position.x >= GetScreenWidth()-32.0f )
        m_position.x = GetScreenWidth()-32.0f;
    if( m_position.y < 32.0f )
        m_position.y = 32.0f;
    if( m_position.y >= GetScreenHeight()-32.0f )
        m_position.y = GetScreenHeight()-32.0f;
}

void Player::draw(bool drawDebugData)
{
    m_pSprite->draw( m_position, 0.0f, m_scale );
}

void Player::onKey(int keyCode, KeyState keyState)
{
    if( keyState == KeyState::Pressed )
    {
        if( keyCode == KEY_LEFT || keyCode == 'A' )
            m_controls.x -= 1;
        if( keyCode == KEY_RIGHT || keyCode == 'D' )
            m_controls.x += 1;
    }

    if( keyState == KeyState::Released )
    {
        if( keyCode == KEY_LEFT )
            m_controls.x += 1;
    }
}

//void Player::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
//{
//}
//
//void Player::OnMouseMovedEvent(float mouseX, float mouseY)
//{
//}

void Player::handleCollision(vec2 collisionPoint, vec2 collisionNormal)
{
}
