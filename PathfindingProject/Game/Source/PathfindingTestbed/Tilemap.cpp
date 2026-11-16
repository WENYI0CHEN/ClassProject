#include "Framework.h"

#include "Tilemap.h"

Tilemap::Tilemap(fw::Mesh* pMesh, fw::ShaderProgram* pShader, ivec2 mapSize, vec2 tilesize, TileType* layout)
    : m_pMesh( pMesh )
    , m_pShader( pShader )
    , m_mapSize( mapSize )
    , m_tileSize( tilesize )
{
    m_map = new TileType[m_mapSize.x * m_mapSize.y];
    for( int y=0; y<m_mapSize.y; y++ )
    {
        for( int x=0; x<m_mapSize.x; x++ )
        {
            int index = y*m_mapSize.x + x;
            int flippedIndex = (m_mapSize.y-1 - y)*m_mapSize.x + x;
            m_map[index] = layout[flippedIndex];
        }
    }

    m_tileInfo[(int)TileType::Ground] = { true,  color4f::green() };
    m_tileInfo[(int)TileType::Wall]   = { false, color4f::darkGray()  };
}

Tilemap::~Tilemap()
{
}

void Tilemap::draw(fw::Camera* pCamera)
{
    for( int y=0; y<m_mapSize.y; y++ )
    {
        for( int x=0; x<m_mapSize.x; x++ )
        {
            int index = y*m_mapSize.x + x;
            TileType type = m_map[index];
            color4f color = m_tileInfo[(int)type].m_color;
            vec2 position = vec2( (float)x, (float)y ) * m_tileSize;

            m_pMesh->draw( pCamera, m_pShader, m_tileSize, 0, position, color, nullptr );
        }
    }
}

bool Tilemap::isTileWalkableForWorldPos(vec2 worldPos)
{
    ivec2 tilePos = { int(worldPos.x / m_tileSize.x), int(worldPos.y / m_tileSize.y) };
    return isTileWalkableForTilePos( tilePos );
}

bool Tilemap::isTileWalkableForTilePos(ivec2 tilePos)
{
    if( tilePos.x < 0 || tilePos.x >= m_mapSize.x || tilePos.y < 0 || tilePos.y >= m_mapSize.y )
        return false;

    int index = tilePos.y * m_mapSize.x + tilePos.x;
    TileType type = m_map[index];
    return m_tileInfo[(int)type].m_walkable;
}

vec2 Tilemap::getWorldPosForTileIndex(int tileIndex)
{
    int x = tileIndex % m_mapSize.x;
    int y = tileIndex / m_mapSize.x;
    return vec2((float)x, (float)y) * m_tileSize + (m_tileSize * 0.5f);
}
