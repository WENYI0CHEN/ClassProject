#pragma once

#include "GameObject.h"

class ProgressBar : public GameObject
{
public:
    ProgressBar(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~ProgressBar();

    virtual void update(float deltaTime) override;
    virtual void draw(fw::Camera* pCamera) override;

    // Setters.
    void setFGColor(const color4f& color) { m_fgColor = color; }
    void setBGColor(const color4f& bgColor) { m_bgColor = bgColor; }
    void setMinValue(float minValue) { m_minValue = minValue; }
    void setMaxValue(float maxValue) { m_maxValue = maxValue; }
    void setSize(const vec2& size) { m_size = size; }
    void setValue(float value);

private:
    color4f m_fgColor = color4f::white();
    color4f m_bgColor = color4f::lightGray();
    float m_minValue = 0.0f;
    float m_maxValue = 100.0f;
    float m_value = 50.0f;
    vec2 m_size = vec2(100, 20);
};
