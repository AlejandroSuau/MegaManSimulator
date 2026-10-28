#pragma once
#include "StateMachineMngr.h"

class ScreenOptionsState : public StateMachineMngr
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

	int getScreenId() { return (int)ScreenState::OPTIONS; }
	
	static ScreenOptionsState& getInstance();

private:
	ScreenOptionsState();
	ScreenOptionsState(const ScreenOptionsState& other);
	ScreenOptionsState& operator= (const ScreenOptionsState& other);

	int framesCounter = 0;
	int finishScreen = 0;
};