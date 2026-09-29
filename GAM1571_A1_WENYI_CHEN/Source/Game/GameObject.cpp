#include "GameObject.h"

GameObject::GameObject(Game* pGame)
    : m_pGame( pGame )
{
}

GameObject::~GameObject()
{
}

void GameObject::reset()
{
}

void GameObject::update(float deltaTime)
{
}

void GameObject::draw(bool drawDebugData)
{
}

bool GameObject::isCollidingWithGameObject(GameObject* pOtherObject)
{
    return false;
}
