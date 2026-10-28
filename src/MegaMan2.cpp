/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#pragma once

#include "raylib.h"
#include "Game/GlobalGameDefines.h"
#include "Game/Managers/GameManager.h"

extern const int screenWidth = 1920;
extern const int screenHeight = 1080;

extern const int gravity = 400;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(GameGlobalVar::screenWidth, GameGlobalVar::screenHeight, "MEGAMAN 2 SIMULATOR");

    GameManager & GameMngr = GameManager::GetGameManager();
    GameMngr.InitGame();
    
        SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        float deltaTime = GetFrameTime();

        //Update Frame
        GameMngr.UpdateFrame(deltaTime);
        
        //Draw Frame
        GameMngr.GetGameManager().DrawFrame();
    }

    // De-Initialization
    GameManager::GetGameManager().UnloadGame();

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}