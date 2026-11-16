#pragma once

#include "GameDataTypes.h"
#include "ivec2.h"

class Game;

struct TileInfo
{
    bool m_walkable;
    color4f m_color;
};

enum class TileType
{
    Ground,
    Wall,
    NumTypes,
};

class Tilemap
{
public:
    Tilemap(fw::Mesh* pMesh, fw::ShaderProgram* pShader, ivec2 mapSize, vec2 tilesize, TileType* layout);
    virtual ~Tilemap();

    void draw(fw::Camera* pCamera);

    vec2 getTileSize() { return m_tileSize; }
    ivec2 getMapSize() { return m_mapSize; }

    bool isTileWalkableForWorldPos(vec2 worldPos);
    bool isTileWalkableForTilePos(ivec2 tilePos);
    vec2 getWorldPosForTileIndex(int tileIndex);

protected:
    ivec2 m_mapSize;
    vec2 m_tileSize;

    TileType* m_map = nullptr;
    TileInfo m_tileInfo[(int)TileType::NumTypes];

    fw::Mesh* m_pMesh = nullptr;
    fw::ShaderProgram* m_pShader = nullptr;
};
