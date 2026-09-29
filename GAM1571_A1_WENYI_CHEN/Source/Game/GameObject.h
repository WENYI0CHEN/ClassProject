#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"

class Game;

class GameObject
{
public:
    GameObject(Game* pGame);
    virtual ~GameObject();

    virtual void reset();
    virtual void update(float deltaTime);
    virtual void draw(bool drawDebugData);
    virtual bool isCollidingWithGameObject(GameObject* pOtherObject);

    // Getters.
    bool isActive()       { return m_active; }
    vec2 getPosition()    { return m_position; }
    float getAngle()      { return m_angle; }
    vec2 getScale()       { return m_scale; }
    float getRadius()     { return m_radius; }
    Color getColor()      { return m_color; }

    // Setters.
    virtual void setActive(bool active) { m_active = active; }
    void setPosition(vec2 pos)    { m_position = pos; }
    void setAngle(float angle)    { m_angle = angle; }
    void setScale(vec2 scale)     { m_scale = scale; }
    void setRadius(float radius)  { m_radius = radius; }
    void setColor(Color color)    { m_color = color; }

protected:
    Game* m_pGame = nullptr;

    bool m_active = false;

    vec2 m_position = 0;
    float m_angle = 0;
    vec2 m_scale = 1;
    float m_radius = 0;
    Color m_color = WHITE;
};
