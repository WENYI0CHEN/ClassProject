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
    reset();
    ivec2 startPos(sx, sy);
    ivec2 endPos(ex, ey);
    if (!m_pTilemap->isTileWalkableForTilePos(startPos) || !m_pTilemap->isTileWalkableForTilePos(endPos))
    {
        return false;
    }

    int start = sy * 10 + sx;
    int end = ey * 10 + ex;
    m_nodes[start].cost = 0;
    m_nodes[start].estimate = abs(endPos.x - startPos.x) + abs(endPos.y - startPos.y);
    m_nodes[start].finalCost = m_nodes[start].cost + m_nodes[start].estimate;
    m_nodes[start].status = PathNode::Status::Open;

    int dx[4] = { 0,-1,1,0 };
    int dy[4] = { 1,0,0,-1 };

    int current = start;
    while (true)
    {
        if (current == end)
            return true;

        m_nodes[current].status = PathNode::Status::Closed;
        int tem = current;
        int cx = current % 10;
        int cy = current / 10;

        for (int d = 0; d < 4; d++)
        {
            int nx=cx+dx[d];
            int ny= cy+dy[d];
            int current = ny * 10 + nx;
            if (!m_pTilemap->isTileWalkableForTilePos(ivec2(nx, ny)))
                continue;
            if (m_nodes[current].status == PathNode::Status::Closed)
                continue;

            if (m_nodes[tem].cost + 1 < m_nodes[current].cost)
            {
                    m_nodes[current].parentNodeIndex = tem;
                    m_nodes[current].cost = m_nodes[tem].cost + 1;
                    m_nodes[current].estimate = abs(endPos.x - nx) + abs(endPos.y - ny);
                    m_nodes[current].finalCost = m_nodes[current].cost + m_nodes[current].estimate;
                    m_nodes[current].status = PathNode::Status::Open;

            }
        }

        int best = -1;
        for (int i = 0; i < m_pTilemap->getMapSize().x * m_pTilemap->getMapSize().y; i++)
        {
            if (m_nodes[i].status == PathNode::Status::Open)
            {
                if (best == -1 || m_nodes[i].finalCost < m_nodes[best].finalCost)
                {
                    best = i;
                }
            }
        }
        if (best == -1)
            return false;

        current = best;
    }
}

std::vector<int> Pathfinder::getPath(int ex, int ey)
{
    std::vector<int>path;
    int current = ey * 10 + ex;

    if (m_nodes[current].cost == FLT_MAX)
        return path;

    while (current != -1)
    {
        path.push_back(current);
        current = m_nodes[current].parentNodeIndex;
    }
    return path;
}
