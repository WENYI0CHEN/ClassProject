#include "Framework.h"
#include "GameObject.h"
#include "GameCamera.h"

GameCamera::GameCamera(Game* pGame)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update(float deltaTime)
{
    if( m_pObjectBeingFollowed )
    {
        m_position = m_pObjectBeingFollowed->getPosition();
        bool isMoving = m_pObjectBeingFollowed->getVelocity().length() > 0.0f;

        float zoomDiff = m_furthestZoom - m_closestZoom;
        float newTargetZoom = m_closestZoom + zoomDiff * isMoving;

        if( isMoving )
        {
            m_delayTimeRemaining = m_delayTime;
            m_targetZoom = newTargetZoom;
        }
        else
        {
            m_delayTimeRemaining -= deltaTime;
            m_targetZoom = m_zoom;
            if( m_delayTimeRemaining < 0.0f )
            {
                m_targetZoom = newTargetZoom;
            }
        }

        // Smoothly interpolate to the target zoom
        float zoomSpeed = 1.0f;
        m_zoom += (m_targetZoom - m_zoom) * zoomSpeed * deltaTime;
    }

    fw::Camera::update( deltaTime );
}

void GameCamera::setObjectToFollow(GameObject* pObject)
{
    m_pObjectBeingFollowed = pObject;
    m_position = m_pObjectBeingFollowed->getPosition();
    m_zoom = m_closestZoom;
    m_targetZoom = m_zoom;
}
