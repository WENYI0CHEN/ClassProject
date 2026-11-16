#include "Framework.h"
#include "ImGuiManager.h"
#include "imgui.h"

#include "Game.h"

Game::Game(fw::FWCore* framework)
    : fw::GameCore( framework )
{
    m_pImGuiManager = new ImGuiManager( framework );

    m_pBasicShader = new fw::ShaderProgram( "Data/Basic.vert", "Data/Basic.frag" );
    m_pTextureShader = new fw::ShaderProgram( "Data/Basic.vert", "Data/Texture.frag" );

    std::vector<fw::VertexFormat> tilePoints = {
        { vec2(0,0), 255,255,255,255, vec2(0,0) },
        { vec2(1,0), 255,255,255,255, vec2(1,0) },
        { vec2(0,1), 255,255,255,255, vec2(0,1) },
        { vec2(1,1), 255,255,255,255, vec2(1,1) }
    };

    m_meshes["tile"] = new fw::Mesh( GL_TRIANGLES, tilePoints );
    m_textures["Grid"] = new fw::Texture();

    reset();
}

Game::~Game()
{
    // Delete all objects.
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
}

void Game::update(float deltaTime)
{
    m_pImGuiManager->startFrame( deltaTime );
}

void Game::draw()
{
    glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    m_pImGuiManager->endFrame();

    m_pFramework->swapBuffers();
}

void Game::onKey(int keyCode, bool pressed)
{
}
