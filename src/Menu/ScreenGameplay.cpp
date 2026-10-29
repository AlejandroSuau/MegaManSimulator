
#pragma once

#include "ScreenGameplay.h"
#include "raylib.h"
#include "Game/Managers/GameManager.h"
#include "Game/GlobalGameDefines.h"

#include <string>
#include <algorithm>

const std::array<Rectangle, 7> ScreenGameplayState::platforms_{
	// Initial floor
	Rectangle{ 0.0f,    429.0f, 860.0f, 56.0f },

	//Small platforms, from left to right.
	Rectangle{ 1341.0f, 322.0f, 110.0f, 56.0f },
	Rectangle{ 1932.0f, 322.0f,  57.0f, 56.0f },
	Rectangle{ 2524.0f, 268.0f,  57.0f, 56.0f },
	Rectangle{ 3062.0f, 322.0f,  56.0f, 56.0f },
	Rectangle{ 3600.0f, 322.0f, 164.0f, 56.0f },

	// Last long floor (until end).
	Rectangle{
		6020.0f,
		429.0f,
		8593.f - 6020.0f,
		56.0f
	}
};

ScreenGameplayState::ScreenGameplayState()
{

}

ScreenGameplayState& ScreenGameplayState::getInstance()
{
	static ScreenGameplayState singleton;
	return singleton;
}

void ScreenGameplayState::InitScreen(void)
{
	framesCounter = 0;
	finishScreen = 0;

	GameManager::GetGameManager().ResetCamera();
	stageTexture_ = LoadTexture("resources/Game/Stages/AirManStage_Backup.png");

}

void ScreenGameplayState::UpdateScreen(float deltaTime)
{
	EvaluateInput(deltaTime);
	DebugOptions();
}

void ScreenGameplayState::DrawScreen(void)
{

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

	GameManager& GameInst = GameManager::GetGameManager();

	const auto camera = GameInst.GetCamera();

	BeginMode2D(camera);

	DrawTexture(stageTexture_, 0, 0, WHITE);
	DrawDebug();

	EndMode2D();
	
	// UI Score, lives

	//Font font = GameInst.GetFont();
	//DrawText("SCORE:", 300.f, 100.f, 25, WHITE);
	//DrawText(to_string(GameInst.GetScore()).c_str(), 440.f, 100.f, 25, WHITE);

	if (debug_floor)
	{
		const auto mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
		DrawText(TextFormat("Mundo: X %.0f | Y %.0f", mouseWorld.x, mouseWorld.y), 20, 20, 20, YELLOW);
	}
}

void ScreenGameplayState::UnloadScreen(void)
{
	UnloadTexture(stageTexture_);
}

int  ScreenGameplayState::FinishScreen(void)
{
	return finishScreen;
}

void ScreenGameplayState::EvaluateInput(float deltaTime)
{
	// Camera movement
	Camera2D& camera = GameManager::GetGameManager().GetCamera();
	
	float direction = 0.f;
	if (IsKeyDown(KEY_LEFT))
	{
		direction -= 1.f;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		direction += 1.f;
	}

	camera.offset.x -= direction * GameGlobalVar::cameraSpeed * deltaTime;

	const float stageWidthOnScreen = static_cast<float>(stageTexture_.width) * camera.zoom;
	const float minOffsetX = std::min(0.f, static_cast<float>(GetScreenWidth() - stageWidthOnScreen));
	camera.offset.x = std::clamp(camera.offset.x, minOffsetX, 0.f);

	if (IsKeyPressed(KEY_ENTER))
	{
		finishScreen = 4;   // END SCREEN
	}
}

void ScreenGameplayState::DebugOptions()
{
	if (IsKeyPressed(KEY_O))
	{
		debug_floor = !debug_floor;
	}
}

void ScreenGameplayState::DrawDebug() const
{
	GameManager& GameInst = GameManager::GetGameManager();
	if (!debug_floor) { return; }

	for (const auto& platform : platforms_)
	{
		DrawRectangleRec(platform, Fade(GRAY, 0.45f));
		DrawRectangleLinesEx(platform, 2.0f, LIME);
	}
}