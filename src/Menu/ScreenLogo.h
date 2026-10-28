#pragma once
#include "StateMachineMngr.h"

#include "raylib.h"

class ScreenLogoState : public StateMachineMngr
{
public:

	void enter(Screen* screen) {}
	void toggle(Screen* screen) {};
	void exit(Screen* screen) {};


	void InitScreen(void);
	void UpdateScreen(float deltaTime);
	void DrawScreen(void);
	void UnloadScreen(void);
	int FinishScreen(void);

	int getScreenId() { return (int)ScreenState::LOGO; }


	static ScreenLogoState& getInstance();

private:
	ScreenLogoState();
	ScreenLogoState(const ScreenLogoState& other);
	ScreenLogoState& operator= (const ScreenLogoState& other);

	float chargeTime_ = 0.f;

	Texture2D logo_;

	float alpha_ = 1.0f;         // Useful for fading
	int finishScreen_ = 0;

};