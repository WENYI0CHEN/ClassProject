#pragma once

class Tilemap;

struct PathNode
{
    enum class Status
    {
        Unchecked,
        Open,
        Closed,
    };

    int parentNodeIndex = 0;
    Status status = Status::Unchecked;

    float cost = 0; // G
    float estimate = 0; // H
    float finalCost = 0; // F
};

class Pathfinder
{
public:
    Pathfinder(Tilemap* pTilemap);
    virtual ~Pathfinder();

    void reset();

    // Start a search, supply the starting tile x,y and ending tile x,y.
    // Returns true if path found, false otherwise.
    bool findPath(int sx, int sy, int ex, int ey);

    // Retrieve the final path, pass a nullptr for "path" to just get length of the path.
    std::vector<int> getPath(int ex, int ey);

protected:
    Tilemap* m_pTilemap;
    PathNode* m_nodes;
};
