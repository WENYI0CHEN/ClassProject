#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
private:
    static const color4f c_color;
    static const float c_speed;

public:
    Bullet(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~Bullet();

    virtual void update(float deltaTime) override;
    virtual void draw(fw::Camera* pCamera) override;

    // Getters.

    // Setters.
    void setDirection(vec2 dir) { m_direction = dir; }

private:
    vec2 m_direction = 0;
};
