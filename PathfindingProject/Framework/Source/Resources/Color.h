#pragma once

namespace fw
{
    class color4f
    {
    public:
        static const color4f blue()      { return { 0.2f, 0.2f, 0.8f, 1.0f }; }
        static const color4f white()     { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
        static const color4f red()       { return { 0.8f, 0.2f, 0.2f, 1.0f }; }
        static const color4f darkRed()   { return { 0.5f, 0.1f, 0.1f, 1.0f }; }
        static const color4f green()     { return { 0.2f, 0.8f, 0.2f, 1.0f }; }
        static const color4f black()     { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
        static const color4f yellow()    { return { 0.8f, 0.8f, 0.2f, 1.0f }; }
        static const color4f cyan()      { return { 0.2f, 0.8f, 0.8f, 1.0f }; }
        static const color4f magenta()   { return { 0.8f, 0.2f, 0.8f, 1.0f }; }
        static const color4f darkGray()  { return { 0.3f, 0.3f, 0.3f, 1.0f }; }
        static const color4f lightGray() { return { 0.7f, 0.7f, 0.7f, 1.0f }; }

    public:
        color4f() : r(1), g(1), b(1), a(1) {}
        color4f(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    public:
        float r;
        float g;
        float b;
        float a;
    };
}
