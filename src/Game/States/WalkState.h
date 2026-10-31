#pragma once

#include "CharacterState.h"

class GameCharacter;

class WalkState : public CharacterState
{
public:
	void updateState(GameCharacter* character);

	CharSpriteDirection GetSpriteState(bool isFacingLeft) const override
	{
		return isFacingLeft ? CharSpriteDirection::State_WalkLeft : CharSpriteDirection::State_WalkRight;
	}

	const char* GetName() const override { return "Walk"; }

	static CharacterState& getInstance();

private:
	WalkState() = default;
	WalkState(const WalkState& other) = delete;
	WalkState& operator=(const WalkState& other) = delete;
};