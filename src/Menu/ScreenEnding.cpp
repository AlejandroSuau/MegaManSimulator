#include "ScreenEnding.h"
#include "Game/Managers/GameManager.h"

#include "raylib.h"

#include <string>


ScreenEndingState& ScreenEndingState::getInstance()
{
	static ScreenEndingState singleton;
	return singleton;
}


void ScreenEndingState::InitScreen(void)
{
	framesCounter = 0;
	finishScreen = 0;
}

void ScreenEndingState::UpdateScreen(float deltaTime)
{
	if (IsKeyPressed(KEY_ENTER) )
	{
		finishScreen = 1; // GAMEPLAY

	}
	if (IsKeyPressed(KEY_O))
	{
		finishScreen = 2;   // OPTIONS
	}
}

void ScreenEndingState::DrawScreen(void)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
	GameManager& GameInst = GameManager::GetGameManager();

	Font font = GameInst.GetFont();

	// Write this in case of win
	DrawTextEx(font, "GAME OVER", Vector2{ (GetScreenWidth() / 2.f) - 150, (GetScreenHeight() / 2.f) -150 }, font.baseSize * 2.0f, 1, RED);

	DrawTextEx(font, "YOU WIN!", Vector2{ (GetScreenWidth() / 2.f) - 150, (GetScreenHeight() / 2.f) - 150 }, font.baseSize * 2.0f, 1, WHITE);

	DrawText("Press Enter for Playing", (GetScreenWidth() / 2.f) - (MeasureText("Press Enter for Playing", 25) / 2), (GetScreenHeight() / 2.f) + 50, 25, WHITE);
	DrawText("Press 'O' for Options", (GetScreenWidth() / 2.f) - (MeasureText("Press 'I' for Instructions", 25) / 2), (GetScreenHeight() / 2.f) + 100, 25, WHITE);

}

void ScreenEndingState::UnloadScreen(void)
{
	//Let's unload all the enemies lines
	GameManager& GameInst = GameManager::GetGameManager();

}

int  ScreenEndingState::FinishScreen(void)
{
	return finishScreen;
}