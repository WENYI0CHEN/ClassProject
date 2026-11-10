#include "Framework.h"
#include "ProgressBar.h"
#include "Helpers/MathFuncs.h"

ProgressBar::ProgressBar(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : GameObject( pGame, pMesh, pShader )
{
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::update(float deltaTime)
{
    float perc = (m_value - m_minValue) / (m_maxValue - m_minValue);
    m_scale = vec2( m_size.x * perc, m_size.y );
}

void ProgressBar::draw(fw::Camera* pCamera)
{
    m_pMesh->draw( pCamera, m_pShader, m_size, m_angle, m_position, m_bgColor, m_pTexture );
    m_pMesh->draw( pCamera, m_pShader, m_scale, m_angle, m_position, m_fgColor, m_pTexture );
}

void ProgressBar::setValue(float value)
{
    m_value = clamp( value, m_minValue, m_maxValue );
}
