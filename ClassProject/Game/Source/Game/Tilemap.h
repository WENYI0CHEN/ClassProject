#pragma once

#include"Framework.h"
#include<cmath>


enum class TileType
{
	Green,
	Blue,
	Count
};

struct TilePro
{
	fw::color4f color;
};


class Tilemap
{
public:
    Tilemap(const TileType* layout, fw::ivec2 mapSize,
        fw::vec2 tileSize, fw::Mesh* pMesh)
        : m_pLayout(new TileType[mapSize.m_x * mapSize.m_y])
        , m_mapSize(mapSize)
        , m_tileSize(tileSize)
        , m_pMesh(pMesh)
    {
        const int tileCount = mapSize.m_x * mapSize.m_y;

        for (int i = 0; i < tileCount; ++i)
        {
            m_pLayout[i] = layout[i];
        }
    }

    ~Tilemap()
    {
        delete[] m_pLayout; 
    }

    fw::vec2 tileToWorld(fw::ivec2 tileCoord) const
    {
        return { tileCoord.m_x * m_tileSize.x, tileCoord.m_y * m_tileSize.y };
    }

    fw::ivec2 worldToTile(fw::vec2 worldPos) const
    {
        return {
            static_cast<int>(std::floor(worldPos.x / m_tileSize.x)),
            static_cast<int>(std::floor(worldPos.y / m_tileSize.y))
        };
    }

    const TilePro* getTileProAtTile(fw::ivec2 tileCoord) const
    {
        if (tileCoord.m_x < 0 || tileCoord.m_x >= m_mapSize.m_x ||
            tileCoord.m_y < 0 || tileCoord.m_y >= m_mapSize.m_y)
        {
            return nullptr;
        }

        const int index = tileCoord.m_y * m_mapSize.m_x + tileCoord.m_x;
        return &m_tilePro[static_cast<int>(m_pLayout[index])];
    }

    const TilePro* getTileProAtWorld(fw::vec2 worldPos) const
    {
        return getTileProAtTile(worldToTile(worldPos));
    }

    void draw(fw::Camera* pCamera, fw::ShaderProgram* pShader) const
    {
        for (int y = 0; y < m_mapSize.m_y; ++y)
        {
            for (int x = 0; x < m_mapSize.m_x; ++x)
            {
                const fw::ivec2 tileCoord{ x, y };
                const fw::vec2 center = tileToWorld(tileCoord) + m_tileSize * 0.5f;
                const TilePro* pProperties = getTileProAtTile(tileCoord);

                m_pMesh->draw(
                    pCamera, pShader,
                    m_tileSize,        
                    0.0f,               
                    center,             
                    pProperties->color, 
                    nullptr);          
            }
        }
    }
private:
    TileType* m_pLayout;
    fw::ivec2 m_mapSize;
    fw::vec2 m_tileSize;
    fw::Mesh* m_pMesh;
    TilePro m_tilePro[static_cast<int>(TileType::Count)] =
    {
        {fw::color4f::green()},
        {fw::color4f::blue()}
    };

};