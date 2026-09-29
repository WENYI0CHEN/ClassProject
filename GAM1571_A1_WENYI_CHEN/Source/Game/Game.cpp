#include "raylib.h"
#include <float.h>

#include "Game.h"
#include "Enemy.h"
#include "Helpers/MathFuncs.h"
#include "Pickup.h"
#include "Player.h"
#include "Helpers/Sprite.h"
#include "Weapon_Gun.h"

const int Game::c_numEnemies = 1000;
const int Game::c_numPickups = 300;
const int Game::c_initialXPRequiredToLevelUp = 10;
const float Game::c_XPMultiplierPerLevel = 1.5f;
const float Game::c_initialEnemySpawnTime = 3.0f;

Game::Game()
{
    m_textures["BG"] = LoadTexture( "Data/Textures/BG.png" );
    m_textures["Player"] = LoadTexture( "Data/Textures/knight_run_anim_f1.png" );
    m_textures["Bat1"] = LoadTexture( "Data/Textures/fly_anim_f1.png" );
    m_textures["Bat2"] = LoadTexture( "Data/Textures/fly_anim_f2.png" );

    // Create the background.
    m_pBG = new Sprite( m_textures["BG"] );

    // Create a player.
    m_pPlayer = new Player( this );

    m_weapons.push_back( new Weapon_Gun( this ) );

    // Fill a vector with enemies.
    for( int i=0; i<c_numEnemies; i++ )
    {
        Enemy* pEnemy = new Enemy( this );
        m_enemies.push_back( pEnemy );
    }

    // Fill a vector with pickups.
    for( int i=0; i<c_numPickups; i++ )
    {
        Pickup* pPickup = new Pickup( this );
        m_pickups.push_back( pPickup );
    }

    reset();
}

Game::~Game()
{
    // Delete all objects.
    delete m_pPlayer;

    for( Weapon* pWeapon : m_weapons )
    {
        delete pWeapon;
    }

    for( Enemy* pEnemy : m_enemies )
    {
        delete pEnemy;
    }

    for( Pickup* pPickup : m_pickups )
    {
        delete pPickup;
    }

    for( auto texturePair : m_textures )
    {
        UnloadTexture( texturePair.second );
    }
}

void Game::reset()
{
    m_gameOver = false;
    m_enemiesKilled = 0;
    m_experiencePoints = 0;
    m_timePlayed = 0;
    m_currentXPRequiredToLevelUp = c_initialXPRequiredToLevelUp;

    m_enemySpawnTimer = 0;

    for( Weapon* pWeapon : m_weapons )
    {
        pWeapon->reset();
    }

    for( Enemy* pEnemy : m_enemies )
    {
        pEnemy->reset();
    }

    for( Pickup* pPickup : m_pickups )
    {
        pPickup->reset();
    }

    m_pPlayer->reset();
    m_pPlayer->setActive( true );
    m_pPlayer->setPosition( { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f } );

    // HACK: Manually spawning 2 enemies for testing:
    m_enemies[0]->setActive( true );
    m_enemies[0]->setPosition( vec2( 100.0f, 100.0f ) );
    m_enemies[1]->setActive( true );
    m_enemies[1]->setPosition( vec2( 100.0f, 150.0f ) );
}

void Game::update(float deltaTime)
{
    if( m_gameOver == false )
    {
        m_timePlayed += deltaTime;
    }

    // Update the player, enemies and pickups.
    if( m_gameOver == false )
    {
        m_pPlayer->update( deltaTime );
    }

    for( Weapon* pWeapon : m_weapons )
    {
        pWeapon->update( deltaTime );
    }

    for( Enemy* pEnemy : m_enemies )
    {
        if( pEnemy->isActive() )
        {
            pEnemy->update( deltaTime );
        }
    }

    for( Pickup* pPickup : m_pickups )
    {
        if( pPickup->isActive() )
        {
            pPickup->update( deltaTime );
        }
    }

    // Update timers and spawn objects if needed.
    if( m_gameOver == false )
    {
        m_enemySpawnTimer -= deltaTime;
        if( m_enemySpawnTimer < 0 )
        {
            m_enemySpawnTimer = c_initialEnemySpawnTime;
            spawnEnemies();
        }
    }

    // Handle collisions.
    handleCollisions();

    // Handle xp, leveling up and weapon upgrades.
    HandleXP();
}

void Game::draw()
{
    //ClearBackground( RAYWHITE );
    //DrawText( "Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY );

    // Draw the bg, active enemies/pickups/weapons and the player.
    m_pBG->setOrigin( { 0.0f, 0.0f } );
    m_pBG->draw( 0, 0, 1 );

    for( Pickup* pPickup : m_pickups )
    {
        if( pPickup->isActive() )
        {
            pPickup->draw( m_drawDebugData );
        }
    }

    for( Weapon* pWeapon : m_weapons )
    {
        pWeapon->draw( m_drawDebugData );
    }

    for( Enemy* pEnemy : m_enemies )
    {
        if( pEnemy->isActive() )
        {
            pEnemy->draw( m_drawDebugData );
        }
    }

    if( m_gameOver == false )
    {
        m_pPlayer->draw( m_drawDebugData );
    }

    // Draw some text.
    if( true )
    {
        char tempString[100];
        snprintf( tempString, 100, "Time: %0.0f", m_timePlayed );
        DrawText( tempString, 200, 5, 32, LIGHTGRAY );

        snprintf( tempString, 100, "Kills: %d", m_enemiesKilled );
        DrawText( tempString, 500, 5, 32, LIGHTGRAY );
    }
}

void Game::onKey(int keyCode, KeyState keyState)
{
    if( keyCode == 'R' && keyState == KeyState::Pressed )
    {
        reset();
    }

    if( keyCode == '1' && keyState == KeyState::Pressed )
    {
        m_drawDebugData = !m_drawDebugData;
    }

    // Debug leveling up.
    if( keyCode == 'X' && keyState == KeyState::Pressed )
    {
        m_experiencePoints += m_currentXPRequiredToLevelUp;
    }

    // Send key events to the player.
    m_pPlayer->onKey( keyCode, keyState );
}

//void Game::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
//{
//}

//void Game::OnMouseMovedEvent(float mouseX, float mouseY)
//{
//}

void Game::spawnEnemies()
{
    for( int i=0; i<10; i++ )
    {
        int side = randInt(0, 3);
        vec2 pos;
        if( side == 0 ) // Left
        {
            pos = vec2(randFloat(-264, -64), randFloat(0, 704));
        }
        else if( side == 1 ) // Right
        {
            pos = vec2(randFloat(1280, 1480), randFloat(0, 704));
        }
        else if( side == 2 ) // Top
        {
            pos = vec2(randFloat(0, 1280), randFloat(704, 904));
        }
        else if( side == 3 ) // Bottom
        {
            pos = vec2(randFloat(0, 1280), randFloat(-264, -64));
        }

        spawnEnemy( pos );
    }
}

void Game::spawnEnemy(vec2 pos)
{
    Enemy* pEnemy = nullptr;

    // Find the first inactive Enemy.
    for( int i=0; i<c_numEnemies; i++ )
    {
        if( m_enemies[i]->isActive() == false )
        {
            pEnemy = m_enemies[i];
            break;
        }
    }
}

void Game::spawnPickup(vec2 pos)
{
    Pickup* pPickup = nullptr;

    // Find the first inactive Pickup.
    for( int i=0; i<c_numPickups; i++ )
    {
        if( m_pickups[i]->isActive() == false )
        {
            pPickup = m_pickups[i];
            break;
        }
    }
}

void Game::handleCollisions()
{
    // Weapons hitting enemies.
    for( Weapon* pWeapon : m_weapons )
    {
        pWeapon->handleCollisions( m_enemies );
    }

    // Enemies hitting the player.
    for( int e=0; e<c_numEnemies; e++ )
    {
        if( m_enemies[e]->isActive() && !m_enemies[e]->isFadingOut() )
        {
            vec2 enemyPos = m_enemies[e]->getPosition();
            float enemyRadius = m_enemies[e]->getRadius();
        }
    }

    // Player hitting pickups.
    for( int p=0; p<c_numPickups; p++ )
    {
        if( m_pickups[p]->isActive() && !m_pickups[p]->isAnimating() )
        {
            vec2 pickupPos = m_pickups[p]->getPosition();
            float pickupRadius = m_pickups[p]->getRadius();

            // Check if they collide with the player.
            vec2 playerPos = m_pPlayer->getPosition();
            float playerPickupRadius = m_pPlayer->getPickupRadius();

            if( (playerPos - pickupPos).length() < playerPickupRadius + pickupRadius )
            {
                m_pickups[p]->pickupObject( playerPos );
                m_experiencePoints++;
            }
        }
    }
}

void Game::HandleXP()
{
    if( m_experiencePoints >= m_currentXPRequiredToLevelUp )
    {
        m_experiencePoints -= m_currentXPRequiredToLevelUp;
        m_currentXPRequiredToLevelUp = (int)(m_currentXPRequiredToLevelUp*c_XPMultiplierPerLevel);

        for( Weapon* pWeapon : m_weapons )
        {
            pWeapon->levelUp();
        }
    }
}

void Game::onEnemyKilled(vec2 location)
{
    spawnPickup( location );
    m_enemiesKilled++;
}

Enemy* Game::getClosestEnemy(vec2 pos)
{
    float closestDistance = FLT_MAX;
    Enemy* pClosestEnemy = nullptr;

    for( int i=0; i<c_numEnemies; i++ )
    {
        if( m_enemies[i]->isActive() && m_enemies[i]->getHealth() > 0 )
        {
            vec2 enemyPos = m_enemies[i]->getPosition();
    
            // Don't target enemies that are offscreen.
            if( enemyPos.x < 0.0f || enemyPos.x > GetScreenWidth() ||
                enemyPos.y < 0.0f || enemyPos.y > GetScreenHeight() )
            {
                continue;
            }
    
            float distance = (m_enemies[i]->getPosition() - pos).length();
            if( distance < closestDistance )
            {
                closestDistance = distance;
                pClosestEnemy = m_enemies[i];
            }
        }
    }

    return pClosestEnemy;
}
