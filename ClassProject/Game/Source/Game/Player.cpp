#include "Framework.h"
#include "Game.h"
#include "Player.h"
#include "Helpers/MathFuncs.h"

const float Player::c_speed = 150.0f; // Pixels per second.
const float Player::c_pickupRadius = 100.0f; // Pixels.
const float Player::c_startingHealth = 100.0f;

Player::Player(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : GameObject( pGame, pMesh, pShader )
    , m_pickupRadius( c_pickupRadius )
    , m_health( c_startingHealth )
{
    m_color = color4f::green();
    m_active = true;

    m_radius = 20.0f;

    m_scale = 40.0f;
}

Player::~Player()
{
}

void Player::reset()
{
    m_health = c_startingHealth;
    m_active = true;
    m_position = vec2( (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f );
    m_velocity = 0;
}

void Player::update(float deltaTime)
{
    vec2 dir = m_controls.getNormalized();
    m_velocity = { dir.x * c_speed, dir.y * c_speed };

    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

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

void Player::draw(fw::Camera* pCamera)
{
    GameObject::draw( pCamera );
}

#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT

void Player::onKey(int keyCode, bool pressed)
{
    if( keyCode == KEY_UP || keyCode == 'W' )
        m_controls.y += pressed ? 1 : -1;
    if( keyCode == KEY_DOWN || keyCode == 'S' )
        m_controls.y += pressed ? -1 : 1;
    if( keyCode == KEY_LEFT || keyCode == 'A' )
        m_controls.x += pressed ? -1 : 1;
    if( keyCode == KEY_RIGHT || keyCode == 'D' )
        m_controls.x += pressed ? 1 : -1;
}

void Player::applyDamage(float damage)
{
    m_health -= damage;
    if( m_health < 0 )
    {
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
