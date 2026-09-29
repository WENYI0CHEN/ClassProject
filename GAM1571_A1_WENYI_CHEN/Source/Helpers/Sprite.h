#pragma once

#include "raylib.h"
#include "Helpers/Vector.h"

class Sprite
{
public:
    Sprite(Texture2D texture);
    ~Sprite();

    void draw(vec2 pos, float angle, vec2 scale);

    // Getters.
    vec2 getOrigin() const { return m_origin; }

    // Setters.
    void setOrigin(vec2 origin) { m_origin = origin; }

private:
    Texture2D m_texture;
    vec2 m_origin = { 0.5f, 0.5f };
};
