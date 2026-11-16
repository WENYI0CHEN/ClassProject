#pragma once

#include "Math/vec2.h"

namespace fw
{
    class Camera
    {
    public:
        Camera();
        virtual ~Camera();

        // Getters.
        vec2 getPosition() const { return m_position; }
        float getZoom() const { return m_zoom; }
        float getAspectRatio() const { return m_aspectRatio; }
        vec2 getProjScale() const { return m_projScale; }

        // Setters.
        void setPosition(vec2 pos) { m_position = pos; }
        void setZoom(float zoom) { m_zoom = zoom; }
        void setAspectRatio(float aspect) { m_aspectRatio = aspect; }

        virtual void update(float deltaTime);

    protected:
        vec2 m_position = vec2(0, 0);
        float m_zoom = 1.0f;
        float m_aspectRatio = 1.0f;

        // Internally set.
        vec2 m_projScale = { 1.0f, 1.0f };
    };
}
