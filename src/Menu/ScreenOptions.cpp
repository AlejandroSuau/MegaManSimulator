#include "ScreenOptions.h"
#include "raylib.h"


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

    const char* line1 = "To move PACMAN use the direction keys or A,D,W,S.";
    const char* line2 = "PACMAN has 3 lifes, every dot eaten increases 10 points";
    const char* line3 = "the big dots adds 50 points to the score.";
    const char* line4 = "To win PACMAN has to eat all the dots";
    const char* line5 = "You loose if the ghost SHADOW kills you 3 times.";
    

    DrawText(line1, (GetScreenWidth() / 2) - (MeasureText(line1, 20) / 2.), 150, 20, WHITE);
    DrawText(line2, (GetScreenWidth() / 2) - (MeasureText(line2, 20) / 2.), 180, 20, WHITE);
    DrawText(line3, (GetScreenWidth() / 2) - (MeasureText(line3, 20) / 2.), 210, 20, WHITE);
    DrawText(line4, (GetScreenWidth() / 2) - (MeasureText(line4, 20) / 2.), 240, 20, WHITE);
    DrawText(line5, (GetScreenWidth() / 2) - (MeasureText(line5, 20) / 2.), 270, 20, WHITE);

    const char* line7 = "Press 'O' to return to Title.";

    DrawText(line7, (GetScreenWidth() / 2.f) - (MeasureText(line7, 20) / 2.), 380, 20, WHITE);

}

void ScreenOptionsState::UnloadScreen(void)
{

}

int ScreenOptionsState::FinishScreen(void)
{
	return finishScreen;
}