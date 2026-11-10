#pragma once

#include "Types.h"

class Game;
class GameObject;

class GameCamera : public fw::Camera
{
public:
    GameCamera(Game* pGame);
    virtual ~GameCamera();

    virtual void update(float deltaTime) override;

    // Setters.
    void setObjectToFollow(GameObject* pObject);
    void setZoomLimits(float closest, float furthest) { m_closestZoom = closest; m_furthestZoom = furthest; }

protected:
    Game* m_pGame = nullptr;

    float m_delayTimeRemaining = 0.0f;
    float m_delayTime = 1.5f;

    GameObject* m_pObjectBeingFollowed = nullptr;
    float m_targetZoom = 0.0f;
    float m_closestZoom = 1.0f / 2.0f; // Show 2 units in each direction.
    float m_furthestZoom = 1.0f / 5.0f; // Show 5 units in each direction.
};
