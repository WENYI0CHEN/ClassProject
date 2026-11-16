#pragma once

#include <unordered_map>

class Tilemap;
class Pathfinder;

class PathfindingTestbedGame : public fw::GameCore
{
public:
    PathfindingTestbedGame(fw::FWCore* framework);
    virtual ~PathfindingTestbedGame();

    void reset();
    virtual void update(float deltaTime) override;
    virtual void draw() override;
    virtual void onKey(int keyCode, bool pressed) override;

private:
    fw::Camera* m_pCamera = nullptr;

    std::unordered_map<std::string, fw::Mesh*> m_meshes;
    fw::ShaderProgram* m_pBasicShader = nullptr;

    Tilemap* m_pTilemap;
    Pathfinder* m_pPathfinder;

    int m_startTileIndex = 0;
    int m_endTileIndex = 0;
    std::vector<int> m_pathFound;
};
