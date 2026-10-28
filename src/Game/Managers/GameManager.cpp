#include "GameManager.h"

#include "raylib.h"

#include "Menu/StateMachineMngr.h"
#include "Menu/ScreenLogo.h"
#include "Menu/ScreenTitle.h"
#include "Menu/ScreenGameplay.h"
#include "Menu/ScreenOptions.h"
#include "Menu/ScreenEnding.h"

#include <stdio.h>              // Standard input-output C library
#include <stdlib.h>             // Memory management functions: malloc(), free()
#include <string.h>             // String manipulation functions: strrchr(), strcmp()

//---------------------------------------------------------------
GameManager* GameManager::GameMngr = nullptr;


GameManager& GameManager::GetGameManager()
{
    if (!GameMngr)
        GameMngr = new GameManager();

    return *GameMngr; 
}

GameManager::GameManager()
{
}


void GameManager::InitGame()
{

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/Font/PacManFont.ttf");

    // Setup and init first screen
    ScreenState = &ScreenTitleState::getInstance();
    ScreenState->InitScreen();
     
}
//-------------------------------------------------------
void GameManager::UpdateFrame(float deltaTime)
{
 // Update
 //----------------------------------------------------------------------------------
    
     if (!onTransition)
    {
        ScreenState->UpdateScreen(deltaTime);
        
        switch (ScreenState->getScreenId())
        {
            case (int) ScreenState::LOGO:
                {
                if (int InputKey = ScreenState->FinishScreen())
                {
                    TransitionToScreen((int)ScreenState::TITLE);
                    ScreenState = &ScreenTitleState::getInstance();
                }
                } break;
            case (int) ScreenState::TITLE:
                {
                if (int InputKey = ScreenState->FinishScreen())
                {
                    if (InputKey == 1)
                    {
                        TransitionToScreen((int)ScreenState::OPTIONS);
                        ScreenState = &ScreenOptionsState::getInstance();
                    }
                    else if (InputKey == 2)
                    {
                        TransitionToScreen((int)ScreenState::GAMEPLAY);
                        ScreenState = &ScreenGameplayState::getInstance();
                    }
                }

                } break;
            case (int) ScreenState::OPTIONS:
                {
                if (int InputKey = ScreenState->FinishScreen())
                {
                    if (InputKey == 1)
                    {
                        TransitionToScreen((int)ScreenState::TITLE);
                        ScreenState = &ScreenTitleState::getInstance();
                    }
                }
                } break;
            case (int)ScreenState::GAMEPLAY:
            {
                if (int InputKey = ScreenState->FinishScreen())
                {
                    if (InputKey == 4)
                    {
                        TransitionToScreen((int)ScreenState::ENDING);
                        ScreenState = &ScreenEndingState::getInstance();
                    }
                }

            } break;
            case (int)ScreenState::ENDING:
            {    
                if (int InputKey = ScreenState->FinishScreen())
                {
                    if (InputKey == 2)
                    {
                        TransitionToScreen((int)ScreenState::OPTIONS);
                        ScreenState = &ScreenOptionsState::getInstance();
                    }
                    else if (InputKey == 1)
                    {
                        //player.InitPlayer();
                        //player.SetPlayerNumLifes(3);
                        //EnemyMngr.InitEnemyManager();

                        TransitionToScreen((int)ScreenState::TITLE);
                        ScreenState = &ScreenTitleState::getInstance();
                    }
                }
            } break;
            default: break;
        }
     }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

}

//-------------------------------------------------------

void GameManager::DrawFrame(void)
{

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw full screen rectangle in front of everything
    if (onTransition)
    {
        DrawTransition();
    }
    else
    {
        ScreenState->DrawScreen();
    }
    EndDrawing();
}

//-------------------------------------------------------

void GameManager::UnloadGame(void)
{
    ScreenState->UnloadScreen();

    ClearGameVar();

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice();     // Close audio context
}


//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
void GameManager::ChangeToScreen(int screen)
{
    // Unload current screen
    ScreenState->UnloadScreen();
    
    // Init next screen
    ScreenState->InitScreen();

}

// Request transition to next screen
void GameManager::TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
void GameManager::UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;
            // Unload current screen
            ScreenState->UnloadScreen();

            // Init next screen
            ScreenState->InitScreen();

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Draw transition effect (full-screen rectangle)
void GameManager::DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

void GameManager::ClearGameVar()
{
    score = 0;
    seconds = 0;
    

}


void GameManager::MapsForwardProgression()
{
    switch (NumLevel)
    {
    case Title:
        NumLevel = Level::Level1;
        break;
    case Level1:
        NumLevel = Level::Level2;
        break;
    case Level2:
        NumLevel = Level::End;
        break;
    default:
        break;
    }
}


void GameManager::MapsBackwardProgression()
{

}

