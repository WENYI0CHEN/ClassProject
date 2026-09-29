#pragma once

#include <functional>

using EasingFunction = std::function<float(float)>;
float easingLinear(float value);
float easingOutElastic(float value);
float easingInBack(float value);

class Tween
{
public:
    Tween(float& reference);

    void update(float deltaTime);

    bool isRunning();

    void start(float target, float duration, float initialDelay, EasingFunction easingFunction = easingLinear);
    void cancel();

    void changeTarget(float newTarget) { m_target = newTarget; }

private:
    float& m_reference;
    float m_elapsed;
    float m_duration;
    float m_start;
    float m_target;
    EasingFunction m_easingFunction;
    bool m_isRunning;
};
