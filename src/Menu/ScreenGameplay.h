#pragma once

#include "StateMachineMngr.h"
#include "Game/MegaMan.h"
#include "Game/Enemies/Copipi.h"
#include "Game/Enemies/Lift.h"
#include "Game/Enemies/Dragon.h"
#include "Game/Managers/GameManager.h"

#include "raylib.h"

#include <array>

class ScreenGameplayState : public StateMachineMngr
{
public:

	void enter(Screen* screen) {}
	void toggle(Screen* screen) {};
	void exit(Screen* screen) {};

	virtual void InitScreen(void);
	virtual void UpdateScreen(float deltaTime);
	virtual void DrawScreen(void);
	virtual void UnloadScreen(void);
	virtual int FinishScreen(void);

	int getScreenId() { return (int)ScreenState::GAMEPLAY; }

	static ScreenGameplayState& getInstance();

private:
	ScreenGameplayState();
	ScreenGameplayState(const ScreenGameplayState& other);
	ScreenGameplayState& operator= (const ScreenGameplayState& other);

	Texture2D stageTexture_{};
	static const std::array<Rectangle, 7> kPlatforms;

	static constexpr int kinitialPlayerLives = 3;
	int playerLives_ = kinitialPlayerLives;
	MegaMan player_;
	Vector2 spawnPosition_{};
	
	static const std::array<Vector2, 5> kCopipiSpawns;
	std::array<Copipi, 5> enemies_{};
	Texture2D copipiTexture_{};

	std::array<Lift, 3> lifts_{};
	Texture2D liftTexture_{};

	std::array<Dragon, 5> dragons_{};
	Texture2D redDragonTexture_{};
	Texture2D blueDragonTexture_{};
	Texture2D hornTexture_{};

	Texture2D playerIconTexture_{};
	Texture2D livesMultiplierTexture_{};

	int finishScreen = 0;
	bool isDrawingDebug_ = false;

	void UpdateCamera(float deltaTime);
	void UpdatePlatforms(float deltaTime, std::span<PlatformCollision> platforms);

	void DebugOptions();
	void DrawDebug() const;

	void RespawnPlayer();
	void LoseLife();
	
	void DrawHUD() const;
	void FinishGame(GameResult result);
};