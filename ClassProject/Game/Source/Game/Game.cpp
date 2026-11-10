#include "Framework.h"
#include "ImGuiManager.h"
#include <float.h>
#include "imgui.h"

#include "Game.h"
#include "Enemy.h"
#include "Helpers/MathFuncs.h"
#include "Pickup.h"
#include "Player.h"
#include "ProgressBar.h"
#include "Weapon_Gun.h"
#include "GameCamera.h"


const int Game::c_numEnemies = 1000;
const int Game::c_numPickups = 300;
const int Game::c_initialXPRequiredToLevelUp = 10;
const float Game::c_XPMultiplierPerLevel = 1.5f;
const float Game::c_initialEnemySpawnTime = 3.0f;

Game::Game(fw::FWCore* framework)
    : fw::GameCore( framework )
{
    m_pImGuiManager = new ImGuiManager( framework );

    m_pBasicShader = new fw::ShaderProgram( "Data/Basic.vert", "Data/Basic.frag" );
    m_pTextureShader = new fw::ShaderProgram( "Data/Basic.vert", "Data/Texture.frag" );

    std::vector<fw::VertexFormat> triangleAttribs = {
        { vec2( 0.5f, 0.0f), 0, 255, 0, 255, vec2(1.0f,0.5f) }, // center right
        { vec2(-0.5f, 0.5f), 0, 255, 0, 255, vec2(0.0f,1.0f) }, // top left
        { vec2(-0.5f,-0.5f), 0, 255, 0, 255, vec2(0.0f,0.0f) }  // bottom left
    };

    std::vector<fw::VertexFormat> circlePoints;
    const int numCirclePoints = 20;
    for( int i=0; i<numCirclePoints; i++ )
    {
        float angle = (float)i/(float)numCirclePoints * 2.0f * PI;
        circlePoints.push_back( { vec2(cosf(angle)*10.0f,sinf(angle)*10.0f), 0, 0, 255, 255 } );
    }

    std::vector<fw::VertexFormat> squarePoints = {
        { vec2(-0.5f,-0.5f), 0, 0, 255, 255 },
        { vec2( 0.5f,-0.5f), 0, 0, 255, 255 },
        { vec2(-0.5f, 0.5f), 0, 0, 255, 255 },
        { vec2( 0.5f, 0.5f), 0, 0, 255, 255 }
    };

    std::vector<fw::VertexFormat> progressBarPoints = {
        { vec2(0,-0.5f), 0, 0, 255, 255 },
        { vec2(1,-0.5f), 0, 0, 255, 255 },
        { vec2(0, 0.5f), 0, 0, 255, 255 },
        { vec2(1, 0.5f), 0, 0, 255, 255 }
    };

    m_meshes["triangle"] = new fw::Mesh( GL_TRIANGLES, triangleAttribs );
    m_meshes["circle"] = new fw::Mesh( GL_TRIANGLE_FAN, circlePoints );
    m_meshes["square"] = new fw::Mesh( GL_TRIANGLE_STRIP, squarePoints );
    m_meshes["progressBar"] = new fw::Mesh( GL_TRIANGLE_STRIP, progressBarPoints );

    m_textures["Grid"] = new fw::Texture();

    //BG
    const TileType G = TileType::Green;
    const TileType B = TileType::Blue;
    const TileType BG[] =
    {
        B,B,G,G,
        B,G,G,G,
        G,G,G,B
    };
    m_pTilemap = new Tilemap(BG,{4,3},{320.0f,240.0f},m_meshes["square"]);

    // Create a camera.
    m_pCamera = new GameCamera( this );
   
    m_pCamera->setPosition(vec2(640.0f, 360.0f));
    m_pCamera->setZoom(1.0f / 720.0f);

    float aspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
    m_pCamera->setAspectRatio( aspectRatio );

    m_pHUDCamera = new fw::Camera();
    m_pHUDCamera->setPosition( vec2((float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f) );
    m_pHUDCamera->setZoom( 1.0f / (float)GetScreenHeight() );
    m_pHUDCamera->setAspectRatio( aspectRatio );

    // Create a player.
    m_pPlayer = new Player( this, m_meshes["triangle"], m_pTextureShader );
    m_pPlayer->setTexture( m_textures["Grid"] );
  

    m_weapons.push_back( new Weapon_Gun( this ) );

    // Fill a vector with enemies.
    for( int i=0; i<c_numEnemies; i++ )
    {
        Enemy* pEnemy = new Enemy( this, m_meshes["square"], m_pBasicShader );
        m_enemies.push_back( pEnemy );
    }

    // Fill a vector with pickups.
    for( int i=0; i<c_numPickups; i++ )
    {
        Pickup* pPickup = new Pickup( this, m_meshes["circle"], m_pBasicShader );
        m_pickups.push_back( pPickup );
    }

    m_pHealthBar = new ProgressBar( this, m_meshes["progressBar"], m_pBasicShader );
    m_pHealthBar->setFGColor( color4f::red() );
    m_pHealthBar->setBGColor( color4f::darkRed() );
    m_pHealthBar->setSize( vec2(200, 20) );
    m_pHealthBar->setPosition( vec2(100, 80) );
    m_pHealthBar->setMinValue( 0 );
    m_pHealthBar->setMaxValue( 100 );
    m_pHealthBar->setValue( 100 );

    m_pXPBar = new ProgressBar( this, m_meshes["progressBar"], m_pBasicShader );
    m_pXPBar->setFGColor( color4f::white() );
    m_pXPBar->setBGColor( color4f::darkGray() );
    m_pXPBar->setSize( vec2(1000, 10) );
    m_pXPBar->setPosition( vec2(100, 50) );
    m_pXPBar->setMinValue( 0 );
    m_pXPBar->setMaxValue( c_initialXPRequiredToLevelUp );
    m_pXPBar->setValue( 0 );

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
    delete m_pTilemap;
    for( const auto& meshPair : m_meshes )
    {
        delete meshPair.second;
    }

    for( const auto& texturePair : m_textures )
    {
        delete texturePair.second;
    }

    delete m_pTextureShader;
    delete m_pBasicShader;
    delete m_pImGuiManager;
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



    //// HACK: Manually spawning 2 enemies for testing:
    //m_enemies[0]->setActive( true );
    //m_enemies[0]->setPosition( vec2( 100.0f, 100.0f ) );
    //m_enemies[1]->setActive( true );
    //m_enemies[1]->setPosition( vec2( 100.0f, 150.0f ) );
}

void Game::update(float deltaTime)
{
    m_pImGuiManager->startFrame( deltaTime );

    //ImGui::Text("TEST");

    if( m_gameOver == false )
    {
        m_timePlayed += deltaTime;
    }

    // Update the player, enemies and pickups.
    if( m_gameOver == false )
    {
        m_pPlayer->update( deltaTime );
        m_pCamera->update( deltaTime );
        m_pHUDCamera->update( deltaTime );

        m_pXPBar->setMaxValue( (float)m_currentXPRequiredToLevelUp );
        m_pXPBar->setValue( (float)m_experiencePoints );
        m_pXPBar->update( deltaTime );
    }

    m_pHealthBar->setValue( m_pPlayer->getHealth() );
    m_pHealthBar->update( deltaTime );

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
    glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    m_pTilemap->draw(m_pCamera, m_pBasicShader);

    //ClearBackground( RAYWHITE );
    //DrawText( "Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY );

    //// Draw the bg, active enemies/pickups/weapons and the player.
    //{
    //    m_meshes["square"]->draw( m_pCamera, m_pBasicShader, vec2(1280,720), 0, vec2(640,360), color4f(0,0,0.4f,1) );
    //}

    for( Pickup* pPickup : m_pickups )
    {
        if( pPickup->isActive() )
        {
            pPickup->draw( m_pCamera );
        }
    }

    for( Weapon* pWeapon : m_weapons )
    {
        pWeapon->draw( m_pCamera );
    }

    for( Enemy* pEnemy : m_enemies )
    {
        if( pEnemy->isActive() )
        {
            pEnemy->draw( m_pCamera );
        }
    }

    if( m_gameOver == false )
    {
        m_pPlayer->draw( m_pCamera );
    }

    // Draw some text.
    if( true )
    {
        m_pHealthBar->draw( m_pHUDCamera );
        m_pXPBar->draw( m_pHUDCamera );

        // TODO: Fix me!!!!

        char tempString[100];
        snprintf( tempString, 100, "Time: %0.0f", m_timePlayed );
        //DrawText( tempString, 200, 5, 32, LIGHTGRAY );

        snprintf( tempString, 100, "Kills: %d", m_enemiesKilled );
        //DrawText( tempString, 500, 5, 32, LIGHTGRAY );

        snprintf( tempString, 100, "XP: %d/%d", m_experiencePoints, m_currentXPRequiredToLevelUp );
        //DrawText( tempString, 800, 5, 32, LIGHTGRAY );
    }

    m_pImGuiManager->endFrame();

    m_pFramework->swapBuffers();
}

void Game::onKey(int keyCode, bool pressed)
{
    if( keyCode == 'R' && pressed )
    {
        reset();
    }

    if( keyCode == '1' && pressed )
    {
        m_drawDebugData = !m_drawDebugData;
    }

    // Debug leveling up.
    if( keyCode == 'X' && pressed )
    {
        m_experiencePoints += m_currentXPRequiredToLevelUp;
    }

    // Send key events to the player.
    m_pPlayer->onKey( keyCode, pressed );
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

    // Activate the Enemy and set it's position.
    if( pEnemy )
    {
        pEnemy->setActive( true );
        pEnemy->setPosition( pos );
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

    // Activate the Pickup and set it's position.
    if( pPickup )
    {
        pPickup->setActive( true );
        pPickup->setPosition( pos );
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

            // Check if they collide with the player.
            vec2 playerPos = m_pPlayer->getPosition();
            float playerRadius = m_pPlayer->getRadius();

            if( (playerPos - enemyPos).length() < playerRadius + enemyRadius )
            {
                m_enemies[e]->applyDamage( 100000 );
                m_pPlayer->applyDamage( 10.0f );
                if( m_pPlayer->getHealth() <= 0 )
                {
                    m_gameOver = true;
                }
            }
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
