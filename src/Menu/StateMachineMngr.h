#pragma once
#include "Screen.h"

enum class ScreenState
{
	LOGO = 0,
	TITLE,
	OPTIONS,
	GAMEPLAY,
	ENDING
};

class Screen;

class StateMachineMngr
{

public:

	virtual ~StateMachineMngr() {}
	
	virtual void enter(Screen* screen) = 0;
	virtual void toggle(Screen* screen) = 0;
	virtual void exit(Screen* screen) = 0;

	virtual void InitScreen(void) = 0;
	virtual void UpdateScreen(float deltaTime) = 0;
	virtual void DrawScreen(void) = 0;
	virtual void UnloadScreen(void) = 0;
	virtual int FinishScreen(void) = 0;
	virtual int getScreenId() = 0;
};