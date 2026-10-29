#pragma once

#include "StateMachineMngr.h"
#include "Game/MegaMan.h"

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

	void UpdateCamera(float deltaTime);
	
	void DebugOptions();
	void DrawDebug() const;

	Texture2D stageTexture_{};
	static const std::array<Rectangle, 7> kPlatforms;

	MegaMan player_;
	Vector2 spawnPosition_{};

	int framesCounter = 0;
	int finishScreen = 0;
	bool mb_ReplayLevel = false;

	bool debug_floor = false;
	bool debug_stairs = false;

};