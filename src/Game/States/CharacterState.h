#pragma once

#include "../Character.h"

class GameCharacter;

//////////////////////////////////////
// State Machine
class CharacterState
{
public:
	virtual ~CharacterState() = default;

	virtual void enter(GameCharacter* light) {}
	virtual void exit(GameCharacter* light) {}

	virtual void updateState(GameCharacter* character) = 0;

	virtual CharSpriteDirection GetSpriteState(bool isFacingLeft) const = 0;
	virtual const char* GetName() const = 0;
};

