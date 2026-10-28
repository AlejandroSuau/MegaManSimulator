#pragma once

#include "../Character.h"

class GameCharacter;

//////////////////////////////////////
// State Machine
class CharacterState
{
public:
	
	
	virtual ~CharacterState() {}

	virtual void enter(GameCharacter* light) = 0;
	virtual void updateState(GameCharacter* light) = 0;
	virtual void exit(GameCharacter* light) = 0;
};

