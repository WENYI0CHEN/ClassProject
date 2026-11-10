#include "Framework.h"
#include "GameObject.h"

GameObject::GameObject(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : m_pGame( pGame )
    , m_pMesh( pMesh )
    , m_pShader( pShader )
{
}

GameObject::~GameObject()
{
}

void GameObject::reset()
{
    setActive( false );
}

void GameObject::update(float deltaTime)
{
}

void GameObject::draw(fw::Camera* pCamera)
{
    m_pMesh->draw( pCamera, m_pShader, m_scale, m_angle, m_position, m_color, m_pTexture );
}

bool GameObject::isCollidingWithGameObject(GameObject* pOtherObject)
{
    return false;
}
