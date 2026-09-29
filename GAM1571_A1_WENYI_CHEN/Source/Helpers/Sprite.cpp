#include <assert.h>
#include "Helpers/Sprite.h"
#include "Helpers/Vector.h"

Sprite::Sprite(Texture2D texture)
    : m_texture( texture )
{
}

Sprite::~Sprite()
{
}

void Sprite::draw(vec2 pos, float angle, vec2 scale)
{
    // Adjust position based on origin.
    vec2 scaledSize = { m_texture.width * scale.x, m_texture.height * scale.y };
    vec2 adjustedPos = pos - scaledSize * m_origin;

    Rectangle sourceRect = { 0, 0, (float)m_texture.width, (float)m_texture.height };
    Rectangle destRect = { adjustedPos.x, adjustedPos.y, scaledSize.x, scaledSize.y };
    vec2 origin = 0; //scaledSize * m_origin;

    DrawTexturePro( m_texture, sourceRect, destRect, origin, angle, WHITE );
}
