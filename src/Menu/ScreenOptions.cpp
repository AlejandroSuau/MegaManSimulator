#include "ScreenOptions.h"

#include "Game/Managers/GameManager.h"

#include "raylib.h"

#include <algorithm>
#include <array>

ScreenOptionsState::ScreenOptionsState()
{

}

ScreenOptionsState& ScreenOptionsState::getInstance()
{
	static ScreenOptionsState singleton;
	return singleton;
}

void ScreenOptionsState::InitScreen(void)
{
	framesCounter = 0;
	finishScreen = 0;

    GameManager::GetGameManager().GetAudio().PlayMusic(MusicTrack::Title);
}

void ScreenOptionsState::UpdateScreen(float deltaTime)
{
	if (IsKeyPressed(KEY_O))
	{
		finishScreen = 1;   // TITLE
	}
}

void ScreenOptionsState::DrawScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

    const Font font = GetFontDefault();
    const auto drawCentered = [&](const char* text, float y, float size, Color color) {
        const auto measured = MeasureTextEx(font, text, size, 1);
        DrawTextEx(font, text, { (GetScreenWidth() - measured.x) * 0.5f, y }, size, 1, color);
    };

    const float centerY = GetScreenHeight() * 0.5f;

    constexpr std::array lines{
        "LEFT / RIGHT - Move.",
        "SPACE - Jump while standing on a platform.",
        "Hold Z - Shoot once. Shots last up to 3 seconds.",
        "Reach the right end of the level and land to win.",
        "You start with 3 lives. Losing all of them ends the game.",
        "You lose a life by falling, touching Copipi or hitting dragon horns.",
        "Copipi takes 3 hits. Touching it also removes the enemy.",
        "Platforms can be crossed from below.",
        "Moving platforms carry you while you stand on them.",
        "Dragon horns are dangerous. Land between them.",
        "O during gameplay - Toggle collision debug.",
    };

    float y = centerY - 240.f;

    for (const char* line : lines) {
        drawCentered(line, y, 22.f, WHITE);
        y += 36.f;
    }

    drawCentered("Press O to return to Title.", GetScreenHeight() - 60.f, 24.f, YELLOW);
}

void ScreenOptionsState::UnloadScreen(void)
{

}

int ScreenOptionsState::FinishScreen(void)
{
	return finishScreen;
}