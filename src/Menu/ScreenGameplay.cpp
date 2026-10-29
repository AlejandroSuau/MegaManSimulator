
#pragma once

#include "ScreenGameplay.h"
#include "raylib.h"
#include "Game/Managers/GameManager.h"
#include "Game/GlobalGameDefines.h"

#include <string>
#include <algorithm>

const std::array<Rectangle, 7> ScreenGameplayState::kPlatforms{
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

	player_.InitGameCharacter();
	const auto& firstPlatform = kPlatforms.front();
	spawnPosition_ = Vector2{
		firstPlatform.x + 180.f,
		firstPlatform.y - player_.GetBounds().height * 0.5f
	};
	player_.Reset(spawnPosition_);
}

void ScreenGameplayState::UpdateScreen(float deltaTime)
{
	UpdateCamera(deltaTime);
	DebugOptions();

	player_.UpdateGameCharacter(deltaTime);
	player_.UpdatePhysics(deltaTime, kPlatforms, static_cast<float>(stageTexture_.width));
	player_.UpdateAnimation(deltaTime);

	const bool didFallOutside = (player_.GetBounds().y > static_cast<float>(stageTexture_.height));
	if (didFallOutside || IsKeyPressed(KEY_R)) {
		player_.Reset(spawnPosition_);
	}

	UpdateCamera(deltaTime);
}

void ScreenGameplayState::DrawScreen(void)
{

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

	GameManager& GameInst = GameManager::GetGameManager();

	const auto camera = GameInst.GetCamera();

	BeginMode2D(camera);

	DrawTexture(stageTexture_, 0, 0, WHITE);
	DrawDebug();
	player_.DrawGameCharacter();

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
	player_.UnloadGameCharacter();
	UnloadTexture(stageTexture_);
}

int  ScreenGameplayState::FinishScreen(void)
{
	return finishScreen;
}

void ScreenGameplayState::UpdateCamera(float deltaTime)
{
	// Camera movement
	Camera2D& camera = GameManager::GetGameManager().GetCamera();
	
	const float screenWidth = static_cast<float>(GetScreenWidth());
	const float stageWidth = static_cast<float>(stageTexture_.width) * camera.zoom;

	const float desiredOffsetX = screenWidth * 0.5f - player_.GetPosition().x * camera.zoom;
	const float minOffsetX = std::min(0.0f, screenWidth - stageWidth);

	camera.offset.x = std::clamp(desiredOffsetX, minOffsetX, 0.0f);
	camera.offset.y = 0.0f;
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

	for (const auto& platform : kPlatforms)
	{
		DrawRectangleRec(platform, Fade(GRAY, 0.45f));
		DrawRectangleLinesEx(platform, 2.0f, LIME);
	}

	DrawRectangleLinesEx(player_.GetBounds(), 2.0f, RED);
	DrawCircleV(player_.GetPosition(), 4.0f, RED);
}