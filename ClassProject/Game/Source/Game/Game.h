#pragma once

#include <assert.h>
#include <string>
#include <unordered_map>
#include <vector>
#include"Tilemap.h"
#include "Types.h"

class Enemy;
class GameCamera;
class Pickup;
class Player;
class ProgressBar;
class Sprite;
class Weapon;
class ImGuiManager;
class Tilemap;

class Game : public fw::GameCore
{
private:
    static const int c_numEnemies;
    static const int c_numPickups;
    static const int c_initialXPRequiredToLevelUp;
    static const float c_XPMultiplierPerLevel;
    static const float c_initialEnemySpawnTime;

public:
    Game(fw::FWCore* framework);
    virtual ~Game();

    void reset();
    virtual void update(float deltaTime) override;
    virtual void draw() override;
    virtual void onKey(int keyCode, bool pressed) override;

    // Input event methods.
    //void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
    //void OnMouseMovedEvent(float mouseX, float mouseY);

    void spawnEnemies();
    void spawnEnemy(vec2 pos);
    void spawnPickup(vec2 pos);

    void handleCollisions();
    void HandleXP();

    void onEnemyKilled(vec2 location);
    Enemy* getClosestEnemy(vec2 pos);

    // Getters.
    Player* getPlayer() { return m_pPlayer; }
    bool isGameOver() { return m_gameOver; }
    fw::ShaderProgram* getShader() { return m_pBasicShader; }

    fw::Mesh* getMesh(const char* meshName) const
    {
        auto it = m_meshes.find(meshName);
        if( it != m_meshes.end() )
        {
            return it->second;
        }
        //assert( false );
        return nullptr;
    }

    fw::Texture* getTexture(const char* textureName) const
    {
        auto it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }
        //assert( false );
        return nullptr;
    }
private:
    ImGuiManager* m_pImGuiManager = nullptr;

    GameCamera* m_pCamera = nullptr;
    fw::Camera* m_pHUDCamera = nullptr;

    bool m_gameOver = false;
    int m_enemiesKilled = 0;
    int m_experiencePoints = 0;
    float m_timePlayed = 0.0f;

    int m_currentXPRequiredToLevelUp = 0;
    float m_enemySpawnTimer = 0.0f;

    bool m_drawDebugData = false;

    Player* m_pPlayer = nullptr;
    std::vector<Weapon*> m_weapons;
    EnemyList m_enemies;
    std::vector<Pickup*> m_pickups;

    ProgressBar* m_pHealthBar = nullptr;
    ProgressBar* m_pXPBar = nullptr;

    std::unordered_map<std::string, fw::Mesh*> m_meshes;
    std::unordered_map<std::string, fw::Texture*> m_textures;
    fw::ShaderProgram* m_pBasicShader = nullptr;
    fw::ShaderProgram* m_pTextureShader = nullptr;
    Tilemap* m_pTilemap=nullptr;
};
