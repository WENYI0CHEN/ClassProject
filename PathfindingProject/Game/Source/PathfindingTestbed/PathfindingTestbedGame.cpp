#include "Framework.h"

#include "GameDataTypes.h"
#include "ivec2.h"

#include "Pathfinder.h"
#include "PathfindingTestbedGame.h"
#include "Tilemap.h"

PathfindingTestbedGame::PathfindingTestbedGame(fw::FWCore* framework)
    : GameCore( framework )
{
    m_pBasicShader = new fw::ShaderProgram( "Data/Basic.vert", "Data/Basic.frag" );

    // Create meshes.
    m_meshes["tile"] = new fw::Mesh( GL_TRIANGLE_STRIP, { {{0,0}}, {{1,0}}, {{0,1}}, {{1,1}} } );
    m_meshes["dot"] = new fw::Mesh( GL_TRIANGLE_STRIP, { {{-0.5f,-0.5f}}, {{0.5f,-0.5f}}, {{-0.5f,0.5f}}, {{0.5f,0.5f}} } );

    // Create a camera.
    float aspectRatio = 1280.0f/720.0f;
    m_pCamera = new fw::Camera();
    m_pCamera->setPosition(  vec2(25,25) );
    m_pCamera->setZoom( 1/50.0f );
    m_pCamera->setAspectRatio( aspectRatio );

    reset();
}

PathfindingTestbedGame::~PathfindingTestbedGame()
{
    for( const auto& meshPair : m_meshes )
    {
        delete meshPair.second;
    }
}

void PathfindingTestbedGame::reset()
{
    delete m_pTilemap;
    delete m_pPathfinder;

    // Create a tilemap.
    TileType layout[100] = { TileType::Ground };
    for( int y=0; y<10; y++ )
    {
        for( int x=0; x<10; x++ )
        {
            if( rand()%3 == 0 )
            {
                layout[y*10 + x] = TileType::Wall;
            }
        }
    }

    m_pTilemap = new Tilemap( m_meshes["tile"], m_pBasicShader, {10,10}, {5.0f,5.0f}, layout);
    m_pPathfinder = new Pathfinder( m_pTilemap );
}

void PathfindingTestbedGame::update(float deltaTime)
{
    m_pCamera->update( deltaTime );

    int startx = 3;
    int starty = 1;
    int endx = 0;
    int endy = 6;
    m_startTileIndex = starty * m_pTilemap->getMapSize().x + startx;
    m_endTileIndex = endy * m_pTilemap->getMapSize().x + endx;
    bool found = m_pPathfinder->findPath( startx, starty, endx, endy );

    m_pathFound = m_pPathfinder->getPath( endx, endy );
}

void PathfindingTestbedGame::draw()
{
    glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glPointSize( 10.0f );

    m_pTilemap->draw( m_pCamera );

    vec2 startPos = m_pTilemap->getWorldPosForTileIndex( m_startTileIndex );
    m_meshes["dot"]->draw( m_pCamera, m_pBasicShader, 2, 0, startPos, color4f::white(), nullptr );
    vec2 endPos = m_pTilemap->getWorldPosForTileIndex( m_endTileIndex );
    m_meshes["dot"]->draw( m_pCamera, m_pBasicShader, 2, 0, endPos, color4f::magenta(), nullptr );

    for( int i=0; i<m_pathFound.size(); i++ )
    {
        int tileIndex = m_pathFound[i];
        vec2 pos = m_pTilemap->getWorldPosForTileIndex( tileIndex );
        m_meshes["dot"]->draw( m_pCamera, m_pBasicShader, 1, 0, pos, color4f::red(), nullptr );
    }

    m_pFramework->swapBuffers();
}

void PathfindingTestbedGame::onKey(int keyCode, bool pressed)
{
    if( pressed == true && keyCode == 'R' )
    {
        reset();
    }
}
