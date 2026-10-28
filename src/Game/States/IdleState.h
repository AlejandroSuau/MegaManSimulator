#pragma once

#include "CharacterState.h"

class GameCharacter;

class IdleState : public CharacterState
{
public:
	void enter(GameCharacter* character);
	void updateState(GameCharacter* character);
	void exit(GameCharacter* character) {}
	static CharacterState& getInstance();

private:
	IdleState() {}
	IdleState(const IdleState& other);
	IdleState& operator=(const IdleState& other);
};