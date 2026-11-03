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

	auto& game = GameManager::GetGameManager();
	game.GetAudio().PlayMusic(
		game.GetGameResult() == GameResult::Won
		? MusicTrack::Victory
		: MusicTrack::GameOver);
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
	const bool won = GameInst.GetGameResult() == GameResult::Won;

	Font font = GetFontDefault();

	// Lambda helper for drawing center
	const auto drawCentered = [&](const char* text, float y, float size, Color color) {
		const auto measured = MeasureTextEx(font, text, size, 2.f);
		const float x = (GetScreenWidth() - measured.x) * 0.5f;
		DrawTextEx(font, text, {x, y}, size, 2.f, color);
	};

	const float centerY = GetScreenHeight() * 0.5f;

	drawCentered(won ? "YOU WIN!" : "GAME OVER", centerY - 140.f, 56.f, won ? GREEN : RED);
	drawCentered("Press Enter for going to Title", centerY + 35.f, 24.f, WHITE);
	drawCentered("Press 'O' for Options", centerY + 115.f, 24.f, WHITE);
}

void ScreenEndingState::UnloadScreen(void)
{
	GameManager::GetGameManager().GetAudio().StopMusic();
}

int  ScreenEndingState::FinishScreen(void)
{
	return finishScreen;
}