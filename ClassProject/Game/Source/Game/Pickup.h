#pragma once

#include "GameObject.h"
#include "Tween.h"

class Pickup : public GameObject
{
private:
    static const float c_size; // Pixels.
    static const color4f c_color;

public:
    Pickup(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~Pickup();

    virtual void update(float deltaTime) override;
    virtual void draw(fw::Camera* pCamera) override;

    void pickupObject(vec2 destPos);
    bool isAnimating() { return m_animating; }

    // Getters.

    // Setters.

private:
    bool m_animating = false;

    Tween m_tweenPosX;
    Tween m_tweenPosY;
};
