#pragma once

#include "CharacterState.h"

class GameCharacter;

class JumpState : public CharacterState
{
public:
	void updateState(GameCharacter* character);

	CharSpriteDirection GetSpriteState(bool isFacingLeft) const override
	{
		return isFacingLeft ? CharSpriteDirection::State_JumpLeft : CharSpriteDirection::State_JumpRight;
	}

	const char* GetName() const override { return "Jump"; }

	static CharacterState& getInstance();

private:
	JumpState() = default;
	JumpState(const JumpState& other) = delete;
	JumpState& operator=(const JumpState& other) = delete;
};