#pragma once
#include "StateMachineMngr.h"

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



    int chargeTime_;

	int framesCounter = 0;
	int finishScreen = 0;

	int logoPositionX = 0;
	int logoPositionY = 0;

	int lettersCount = 0;

	int topSideRecWidth = 0;
	int leftSideRecHeight = 0;

	int bottomSideRecWidth = 0;
	int rightSideRecHeight = 0;

	int state = 0;              // Logo animation states
	float alpha = 1.0f;         // Useful for fading

};