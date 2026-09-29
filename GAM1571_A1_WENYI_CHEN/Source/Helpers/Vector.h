#pragma once

#include "raylib.h"
#include <cmath>

class vec2
{
public:
    vec2() {}
    vec2(float nxy) : x(nxy), y(nxy) {}
    vec2(float nx, float ny) : x(nx), y(ny) {}

    void set(float nx, float ny) { x = nx; y = ny; }

    vec2 operator+(vec2 o) { return vec2(x + o.x, y + o.y); }
    vec2 operator-(vec2 o) { return vec2(x - o.x, y - o.y); }
    vec2 operator*(vec2 o) { return vec2(x * o.x, y * o.y); }
    void operator+=(vec2 o) { x += o.x; y += o.y; }

    vec2 operator*(float o) { return vec2(x * o, y * o); }

    operator Vector2() const
    {
        Vector2 v = { x, y };
        return v;
    }

    float length() { return sqrtf( x*x + y*y ); }
    float dot(vec2 o) { return x*o.x + y*o.y; }

    void normalize()
    {
        float len = length();
        if( len == 0 )
            return;
        x /= len;
        y /= len;
    }

    vec2 getNormalized() const
    {
        vec2 v = *this;
        v.normalize();
        return v;
    }

public:
    float x = 0;
    float y = 0;
};
