#pragma once

#include "CharacterState.h"

class GameCharacter;

class IdleState : public CharacterState
{
public:
	void updateState(GameCharacter* character);

	CharSpriteDirection GetSpriteState(bool isFacingLeft) const override
	{
		return isFacingLeft ? CharSpriteDirection::State_IdleLeft : CharSpriteDirection::State_IdleRight;
	}

	const char* GetName() const override { return "Idle"; }

	static CharacterState& getInstance();

private:
	IdleState() = default;
	IdleState(const IdleState& other) = delete;
	IdleState& operator=(const IdleState& other) = delete;
};