
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
	Rectangle{ 0.f, 455.f, 1021.f, 56.f },

	//Small platforms, from left to right.
	Rectangle{ 1477.f, 341.f,  97.f, 56.f },
	Rectangle{ 2014.f, 343.f,  48.f, 54.f },
	Rectangle{ 2517.f, 288.f,  48.f, 52.f },
	Rectangle{ 2996.f, 343.f,  46.f, 54.f },
	Rectangle{ 3501.f, 341.f, 148.f, 56.f },

	// Last long floor (until end).
	Rectangle{ 5336.f, 456.f, 7588.f - 5336.f, 55.f }
};

const std::array<Vector2, 5> ScreenGameplayState::kCopipiSpawns{
    Vector2{  890.f, 400.f },
    Vector2{ 1710.f, 288.f },
    Vector2{ 2806.f, 288.f },
    Vector2{ 3453.f, 286.f },
    Vector2{ 6025.f, 401.f }
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
	
	auto& gameManager = GameManager::GetGameManager();
	gameManager.GetAudio().PlayMusic(MusicTrack::Stage);
	gameManager.SetGameResult(GameResult::Playing);
	gameManager.ResetCamera();

	stageTexture_ = LoadTexture("resources/Game/Stages/AirManStage.png");

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
	lifts_[0].Init(3751.f, 4199.f, 341.f, false, liftTexture_);
	lifts_[1].Init(4274.f, 4722.f, 491.f, true,  liftTexture_);
	lifts_[2].Init(4798.f, 5246.f, 341.f, false, liftTexture_);

	// Dragons
	redDragonTexture_ = LoadTexture("resources/Game/Enemies/RedDemon.png");
	blueDragonTexture_ = LoadTexture("resources/Game/Enemies/BlueDemon.png");
	hornTexture_ = LoadTexture("resources/Game/Enemies/DragonHorn.png");

	dragons_[0].Init(1248.f, 262.f, 402.f, false, redDragonTexture_, hornTexture_);
	dragons_[1].Init(1784.f, 370.f, 460.f, true,  blueDragonTexture_, hornTexture_);
	dragons_[2].Init(2291.f, 233.f, 323.f, false, redDragonTexture_, hornTexture_);
	dragons_[3].Init(2784.f, 256.f, 346.f, true,  blueDragonTexture_, hornTexture_);
	dragons_[4].Init(3268.f, 345.f, 435.f, false, redDragonTexture_, hornTexture_);

	// HUD
	playerIconTexture_ = LoadTexture("resources/UI/MegaManIcon.png");
	livesMultiplierTexture_ = LoadTexture("resources/UI/guiX.png");
}

void ScreenGameplayState::UpdateScreen(float deltaTime)
{
	DebugOptions();

	player_.UpdateGameCharacter(deltaTime);

	// Group together all platforms so we can process their collision together
	// 7 static + 3 lift + 5 dragons
	std::array<PlatformCollision, 7 + 3 + 5> collisionPlatforms{};
	UpdatePlatforms(deltaTime, collisionPlatforms);
	player_.UpdatePhysics(deltaTime, collisionPlatforms, static_cast<float>(stageTexture_.width));

	// Check if player felt
	const bool didFallOutside = (player_.GetBounds().y > static_cast<float>(stageTexture_.height));
	if (didFallOutside) {
		LoseLife();
		return;
	}

	// Check dragon horns
	for (auto& dragon : dragons_) {
		dragon.UpdateHorns(player_.GetPosition());

		if (dragon.TouchesHorns(player_.GetBounds())) {
			LoseLife();
			return;
		}
	}

	// Try to activate enemies
	for (auto& enemy : enemies_) {
		enemy.TryActivate(player_.GetPosition());
		enemy.UpdateGameCharacter(deltaTime);
	}

	player_.UpdateShooting(deltaTime);

	// ChecK if an enemy got hit
	for (auto& enemy : enemies_) {
		if (enemy.IsAlive() 
			&& enemy.IsActive()
			&& player_.ConsumeProjectileHit(enemy.GetBounds())) {
			enemy.TakeHit();
			GameManager::GetGameManager().GetAudio().PlaySoundEffect(SoundType::EnemyHit);
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

	// Check if win
	const auto playerBounds = player_.GetBounds();
	const float levelWidth = static_cast<float>(stageTexture_.width);
	const bool reachedEnd = playerBounds.x + playerBounds.width >= levelWidth - 0.5f;

	if (reachedEnd) {
		FinishGame(GameResult::Won);
		return;
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

	for (auto& dragon : dragons_) {
		dragon.DrawGameCharacter();
	}

	for (auto& enemy : enemies_) {
		enemy.DrawGameCharacter();
	}

	player_.DrawGameCharacter();
	DrawDebug();

	EndMode2D();

	DrawHUD();

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
	UnloadTexture(playerIconTexture_);
	UnloadTexture(livesMultiplierTexture_);

	UnloadTexture(stageTexture_);

	UnloadTexture(copipiTexture_);
	UnloadTexture(liftTexture_);

	UnloadTexture(redDragonTexture_);
	UnloadTexture(blueDragonTexture_);
	UnloadTexture(hornTexture_);

	GameManager::GetGameManager().GetAudio().StopMusic();
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

	for (const auto& dragon : dragons_) {
		DrawRectangleLinesEx(dragon.GetBounds(), 2.f, GREEN);

		if (dragon.AreHornsVisible()) {
			for (const auto& horn : dragon.GetHornBounds()) {
				DrawRectangleRec(horn, Fade(RED, 0.25f));
				DrawRectangleLinesEx(horn, 2.f, RED);
			}
		}
	}
}

void ScreenGameplayState::RespawnPlayer()
{
	player_.Reset(spawnPosition_);

	for (auto& dragon : dragons_) {
		dragon.Reset();
	}

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
	GameManager::GetGameManager().GetAudio().PlaySoundEffect(SoundType::Death);
	if (playerLives_ == 0) {
		FinishGame(GameResult::Lost);
		return;
	}

	RespawnPlayer();
}

void ScreenGameplayState::UpdatePlatforms(
	float deltaTime,
	std::span<PlatformCollision> platforms)
{
	std::size_t index = 0;
	for (const auto& bounds : kPlatforms) {
		platforms[index++] = PlatformCollision {bounds, {0.f, 0.f} };
	}

	// Little lambda helper for update each platform and set its collision
	const auto appendMoving = [&](auto& platform) {
		const auto previous = platform.GetBounds();
		platform.UpdateGameCharacter(deltaTime);

		const auto current = platform.GetBounds();
		platforms[index++] = PlatformCollision {
			current, { current.x - previous.x, current.y - previous.y }
		};
	};

	for (auto& lift : lifts_) {
		appendMoving(lift);
	}

	for (auto& dragon : dragons_) {
		appendMoving(dragon);
	}
}

void ScreenGameplayState::DrawHUD() const
{
	const float margin = 50.f;

	const float x = margin;
	const float y = margin;

	DrawText(
		"Dev by: Alejandro Suau",
		static_cast<int>(x),
		static_cast<int>(y - 20.f),
		30,
		WHITE);

	DrawTextureV(playerIconTexture_, { x, y + 16.f }, WHITE);
	DrawTextureV(livesMultiplierTexture_, { x + 100, y + 48.f }, WHITE);
	
	DrawText(
		TextFormat("%d", playerLives_),
		static_cast<int>(x + 175),
		static_cast<int>(y + 46.f),
		30,
		WHITE);
}

void ScreenGameplayState::FinishGame(GameResult result)
{
	if (finishScreen != 0) { return; }

	GameManager::GetGameManager().SetGameResult(result);
	finishScreen = 4;
}