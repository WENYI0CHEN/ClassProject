#include "Tween.h"
#include "Helpers/MathFuncs.h"

float easingLinear(float x)
{
    return x;
}

float easingOutElastic(float x)
{
    return powf(2, -10 * x) * sinf((x * 10 - 0.75f) * (2 * PI) / 3) + 1;
}

float easingInBack(float x)
{
    return (1.70158f + 1) * x * x * x - 1.70158f * x * x;
}

Tween::Tween(float& reference) :
    m_reference( reference ),
    m_elapsed( 0.0f ),
    m_duration( 0.0f ),
    m_start( 0.0f ),
    m_target( 0.0f ),
    m_easingFunction( easingLinear ),
    m_isRunning( false )
{
}

void Tween::update(float deltaTime)
{
    if( m_isRunning )
    {
        if( m_elapsed < m_duration )
        {
            m_elapsed += deltaTime;
            float elapsed = m_elapsed;
            if( m_elapsed > m_duration )
            {
                m_isRunning = false;
            }
            elapsed = clamp( elapsed, 0, m_duration );

            float pct = elapsed / m_duration;
            m_reference = m_start + (m_target - m_start) * m_easingFunction(pct);
        }
    }
}

bool Tween::isRunning()
{
    return m_isRunning;
}

void Tween::start(float target, float duration, float initialDelay, EasingFunction easingFunction)
{
    m_elapsed = -initialDelay;
    m_duration = duration;
    m_start = m_reference;
    m_target = target;
    m_easingFunction = easingFunction;
    m_isRunning = true;
}

void Tween::cancel()
{
    m_elapsed = 0.0f;
    m_duration = 0.0f;
    m_start = 0.0f;
    m_target = 0.0f;
    m_easingFunction = easingLinear;
    m_isRunning = false;
}
