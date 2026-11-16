#include "Framework.h"
#include "Tilemap.h"
#include "Pathfinder.h"

Pathfinder::Pathfinder(Tilemap* pTilemap)
{
    m_pTilemap = pTilemap;

    ivec2 mapSize = m_pTilemap->getMapSize();
    m_nodes = new PathNode[mapSize.x * mapSize.y];

    reset();
}

Pathfinder::~Pathfinder()
{
    delete[] m_nodes;
}

void Pathfinder::reset()
{
    ivec2 mapSize = m_pTilemap->getMapSize();
    for( int i=0; i<mapSize.x*mapSize.y; i++ )
    {
        m_nodes[i].parentNodeIndex = -1;
        m_nodes[i].status = PathNode::Status::Unchecked;

        m_nodes[i].cost = FLT_MAX; // Set cost to be highest cost possible, so any comparison will be better.
        m_nodes[i].estimate = 0;
        m_nodes[i].finalCost = FLT_MAX;
    }
}

bool Pathfinder::findPath(int sx, int sy, int ex, int ey)
{
    return false;
}

std::vector<int> Pathfinder::getPath(int ex, int ey)
{
    return {};
}
