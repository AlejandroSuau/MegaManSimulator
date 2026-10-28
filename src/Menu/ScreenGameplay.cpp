
#pragma once

#include "ScreenGameplay.h"
#include "raylib.h"
#include "Game/Managers/GameManager.h"
#include "Game/GlobalGameDefines.h"

#include <string>
#include <algorithm>

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

	
}

void ScreenGameplayState::DrawScreen(void)
{

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

	GameManager& GameInst = GameManager::GetGameManager();

	const auto camera = GameInst.GetCamera();

	BeginMode2D(camera);

	DrawTexture(stageTexture_, 0, 0, WHITE);

	EndMode2D();
	
	// UI Score, lives

	Font font = GameInst.GetFont();

	DrawText("SCORE:", 300.f, 100.f, 25, WHITE);
	DrawText(to_string(GameInst.GetScore()).c_str(), 440.f, 100.f, 25, WHITE);
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

}

void ScreenGameplayState::DrawDebug()
{
	GameManager& GameInst = GameManager::GetGameManager();

}