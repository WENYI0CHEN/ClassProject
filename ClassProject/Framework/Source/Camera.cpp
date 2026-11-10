#include "Core/CoreHeaders.h"

#include "Camera.h"
#include "Math/vec2.h"

namespace fw
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::update(float deltaTime)
    {
        m_projScale = vec2( m_zoom, m_zoom * m_aspectRatio );
    }
}
