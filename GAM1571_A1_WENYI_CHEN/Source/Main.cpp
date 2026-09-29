#include "raylib.h"

#include "Game/Game.h"
#include "Helpers/InputTypes.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

int screenWidth = 1280;
int screenHeight = 720;

Game* game = nullptr;

void UpdateDrawFrame();

int main()
{
    InitWindow( screenWidth, screenHeight, "raylib basic window" );

    game = new Game();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop( UpdateDrawFrame, 0, 1 );
#else
    SetTargetFPS( 60 );

    // Main game loop
    while( !WindowShouldClose() )
    {
        UpdateDrawFrame();
    }
#endif

    delete game;

    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
    // Deal with keyboard events.
    for( int i=0; i<256; i++ )
    {
        if( IsKeyPressed(i) )
        {
            game->onKey( i, KeyState::Pressed );
        }
        else if( IsKeyDown(i) )
        {
            game->onKey( i, KeyState::Held );
        }
        if( IsKeyReleased(i) )
        {
            game->onKey( i, KeyState::Released );
        }
    }

    // Update game state.
    game->update( 1/60.0f );

    // Draw the game.
    BeginDrawing();
    game->draw();
    EndDrawing();
}
