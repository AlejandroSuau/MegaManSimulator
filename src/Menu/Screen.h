
#pragma once

class StateMachineMngr;

class Screen
{
public:
	Screen();
	// Same as before
	inline StateMachineMngr* getCurrentState() const { return currentState; }
	// In here, we'll delegate the state transition to the currentState
	void toggle();
	// This will get called by the current state
	void setState(StateMachineMngr& newState);

private:
	// LightState here is now a class, not the enum that we saw earlier
	StateMachineMngr* currentState;
};