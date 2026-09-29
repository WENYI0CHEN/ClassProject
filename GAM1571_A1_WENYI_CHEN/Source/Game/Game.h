#pragma once

#include <assert.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "Helpers/InputTypes.h"
#include "Helpers/Vector.h"
#include "Types.h"

class Enemy;
class Pickup;
class Player;
class Sprite;
class Weapon;

class Game
{
private:
    static const int c_numEnemies;
    static const int c_numPickups;
    static const int c_initialXPRequiredToLevelUp;
    static const float c_XPMultiplierPerLevel;
    static const float c_initialEnemySpawnTime;

public:
    Game();
    virtual ~Game();

    void reset();
    void update(float deltaTime);
    void draw();

    // Input event methods.
    void onKey(int keyCode, KeyState keyState);
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

    Texture2D getTexture(const char* textureName) const
    {
        auto it = m_textures.find( textureName );
        if( it != m_textures.end() )
        {
            return it->second;
        }
        assert( false );
        return Texture2D(); // Return an empty texture if not found.
    }

private:
    bool m_gameOver = false;
    int m_enemiesKilled = 0;
    int m_experiencePoints = 0;
    float m_timePlayed = 0.0f;

    int m_currentXPRequiredToLevelUp = 0;
    float m_enemySpawnTimer = 0.0f;

    bool m_drawDebugData = false;

    Sprite* m_pBG = nullptr;
    Player* m_pPlayer = nullptr;
    std::vector<Weapon*> m_weapons;
    EnemyList m_enemies;
    std::vector<Pickup*> m_pickups;

    std::unordered_map<std::string, Texture2D> m_textures;
};
