
#pragma once

#include "ScreenGameplay.h"
#include "Game/Managers/GameManager.h"
#include "Game/GlobalGameDefines.h"
#include "Game/States/CharacterState.h"

#include "raylib.h"

#include <string>
#include <algorithm>
#include <cmath>

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

const std::array<Vector2, 5> ScreenGameplayState::kCopipiSpawns{
	Vector2{750.f, 374.f},
	Vector2{1600.f, 267.f},
	Vector2{2850.f, 267.f},
	Vector2{3550.f, 267.f},
	Vector2{6500.f, 374.f}
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
	finishScreen = 0;

	GameManager::GetGameManager().ResetCamera();
	stageTexture_ = LoadTexture("resources/Game/Stages/AirManStage_Backup.png");

	// Player
	player_.InitGameCharacter();
	playerLives_ = kinitialPlayerLives;
	const auto& firstPlatform = kPlatforms.front();
	spawnPosition_ = Vector2{
		firstPlatform.x + 180.f,
		firstPlatform.y - player_.GetBounds().height * 0.5f
	};
	player_.Reset(spawnPosition_);

	// Enemies
	copipiTexture_ = LoadTexture("resources/Game/Enemies/Copipi.png");
	for (std::size_t i = 0; i < enemies_.size(); ++i) {
		enemies_[i].Init(kCopipiSpawns[i], copipiTexture_);
	}

	// Lifts
	liftTexture_ = LoadTexture("resources/Game/Enemies/Lift.png");
	lifts_[0].Init(3900.f, 4500.f, 322.f, false, liftTexture_);
	lifts_[1].Init(4600.f, 5200.f, 600.f, true,  liftTexture_);
	lifts_[2].Init(5300.f, 5900.f, 322.f, false, liftTexture_);
}

void ScreenGameplayState::UpdateScreen(float deltaTime)
{
	DebugOptions();

	player_.UpdateGameCharacter(deltaTime);

	UpdateLifts(deltaTime);

	// Group together all platforms so we can process their collision together
	std::array<Rectangle, kPlatforms.size() + lifts_.size()> collisionPlatforms;
	std::copy(kPlatforms.begin(), kPlatforms.end(), collisionPlatforms.begin());
	for (std::size_t i = 0; i < lifts_.size(); ++i) {
		collisionPlatforms[kPlatforms.size() + i] = lifts_[i].GetBounds();
	}

	player_.UpdatePhysics(deltaTime, collisionPlatforms, static_cast<float>(stageTexture_.width));

	// Check if player felt
	const bool didFallOutside = (player_.GetBounds().y > static_cast<float>(stageTexture_.height));
	if (didFallOutside) {
		LoseLife();
		return;
	}

	// Try to activate enemies
	for (auto& enemy : enemies_) {
		enemy.TryActivate(player_.GetPosition());
		enemy.UpdateGameCharacter(deltaTime);
	}

	player_.UpdateShooting(deltaTime);

	// ChecK if an enemy got hit
	for (auto& enemy : enemies_) {
		if (enemy.IsAlive() && player_.ConsumeProjectileHit(enemy.GetBounds())) {
			enemy.TakeHit();
		}
	}

	// Check if enemy died
	for (auto& enemy : enemies_) {
		if (enemy.IsAlive() && CheckCollisionRecs(player_.GetBounds(), enemy.GetBounds())) {
			enemy.Kill();
			LoseLife();
			return;
		}
	}

	player_.updateState();
	player_.UpdateAnimation(deltaTime);

	UpdateCamera(deltaTime);
}

void ScreenGameplayState::DrawScreen(void)
{

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

	GameManager& GameInst = GameManager::GetGameManager();

	const auto camera = GameInst.GetCamera();

	BeginMode2D(camera);

	DrawTexture(stageTexture_, 0, 0, WHITE);
	
	for (auto& lift : lifts_) {
		lift.DrawGameCharacter();
	}

	for (auto& enemy : enemies_) {
		enemy.DrawGameCharacter();
	}

	player_.DrawGameCharacter();
	DrawDebug();

	EndMode2D();

	DrawText(
		TextFormat("Vidas: %d", playerLives_),
		20, 80, 24, WHITE);

	if (isDrawingDebug_)
	{
		const auto mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
		DrawText(TextFormat("Mundo: X %.0f | Y %.0f", mouseWorld.x, mouseWorld.y), 20, 20, 20, YELLOW);
	
		const auto* state = player_.getCurrentState();
		DrawText(TextFormat("Estado: %s", state ? state->GetName() : "None"), 20, 48, 20, YELLOW);
	}
}

void ScreenGameplayState::UnloadScreen(void)
{
	player_.UnloadGameCharacter();
	UnloadTexture(stageTexture_);
	UnloadTexture(copipiTexture_);
	UnloadTexture(liftTexture_);
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
		isDrawingDebug_ = !isDrawingDebug_;
	}
}

void ScreenGameplayState::DrawDebug() const
{
	GameManager& GameInst = GameManager::GetGameManager();
	if (!isDrawingDebug_) { return; }

	for (const auto& platform : kPlatforms)
	{
		DrawRectangleRec(platform, Fade(GRAY, 0.45f));
		DrawRectangleLinesEx(platform, 2.0f, LIME);
	}

	DrawRectangleLinesEx(player_.GetBounds(), 2.0f, RED);
	DrawCircleV(player_.GetPosition(), 4.0f, RED);

	for (const auto& enemy : enemies_) {
		if (!enemy.IsAlive()) { continue; }

		const auto bounds = enemy.GetBounds();

		DrawRectangleLinesEx(bounds, 2.f, ORANGE);
		DrawText(
			TextFormat("HP: %d", enemy.GetHealth()),
			static_cast<int>(bounds.x),
			static_cast<int>(bounds.y - 22.f),
			20,
			YELLOW);
	}

	for (const auto& lift : lifts_) {
		const Rectangle bounds = lift.GetBounds();

		DrawRectangleRec(bounds, Fade(SKYBLUE, 0.35f));
		DrawRectangleLinesEx(bounds, 2.f, BLUE);
	}
}

void ScreenGameplayState::RespawnPlayer()
{
	player_.Reset(spawnPosition_);

	for (auto& enemy : enemies_) {
		if (enemy.IsAlive()) {
			enemy.ResetPosition();
		}
	}

	for (auto& lift : lifts_) {
		lift.Reset();
	}

	UpdateCamera(0.f);
}

void ScreenGameplayState::LoseLife()
{
	if (playerLives_ <= 0) { return; }

	--playerLives_;
	if (playerLives_ == 0) {
		finishScreen = 4;
		return;
	}

	RespawnPlayer();
}

void ScreenGameplayState::UpdateLifts(float deltaTime)
{
	const auto playerBounds = player_.GetBounds();
	const float playerBottom = playerBounds.y + playerBounds.height;

	bool carried = false;
	for (auto& lift : lifts_) {
		// Check previous lift position
		const Rectangle previousBounds = lift.GetBounds();

		lift.UpdateGameCharacter(deltaTime);

		const bool isOverlappingX =
			playerBounds.x + playerBounds.width > previousBounds.x &&
			playerBounds.x < previousBounds.x + previousBounds.width;

		const bool isOnTop = std::abs(playerBottom - previousBounds.y) < 0.01f;

		if (!carried && player_.IsGrounded() && isOverlappingX && isOnTop) {
			auto position = player_.GetPosition();
			position.x += lift.GetDisplacementX();

			player_.SetPosition(position);
			carried = true;
		}
	}
}