#pragma once

#include "GameDataTypes.h"

class Game;

class GameObject
{
public:
    GameObject(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~GameObject();

    virtual void reset();
    virtual void update(float deltaTime);
    virtual void draw(fw::Camera* pCamera);
    virtual bool isCollidingWithGameObject(GameObject* pOtherObject);

    // Getters.
    bool isActive()       { return m_active; }
    vec2 getPosition()    { return m_position; }
    vec2 getVelocity()    { return m_velocity; }
    float getAngle()      { return m_angle; }
    vec2 getScale()       { return m_scale; }
    float getRadius()     { return m_radius; }
    color4f getColor()    { return m_color; }

    // Setters.
    virtual void setActive(bool active)   { m_active = active; }
    void setPosition(vec2 pos)            { m_position = pos; }
    void setVelocity(vec2 vel)            { m_velocity = vel; }
    void setAngle(float angle)            { m_angle = angle; }
    void setScale(vec2 scale)             { m_scale = scale; }
    void setRadius(float radius)          { m_radius = radius; }
    void setColor(color4f color)          { m_color = color; }
    void setTexture(fw::Texture* texture) { m_pTexture = texture; }

protected:
    Game* m_pGame = nullptr;

    bool m_active = false;

    fw::Mesh* m_pMesh = nullptr;
    fw::ShaderProgram* m_pShader = nullptr;
    fw::Texture* m_pTexture = nullptr;

    vec2 m_position = 0;
    vec2 m_velocity = 0;
    float m_angle = 0;
    vec2 m_scale = 1;
    float m_radius = 0;
    color4f m_color = color4f::white();
};
