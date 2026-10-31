#pragma once

#include "CharacterState.h"

class GameCharacter;

class WalkState : public CharacterState
{
public:
	void updateState(GameCharacter* character);

	CharSpriteDirection GetSpriteState(
		bool isFacingLeft,
		bool isShooting) const override;

	const char* GetName() const override { return "Walk"; }

	static CharacterState& getInstance();

private:
	WalkState() = default;
	WalkState(const WalkState& other) = delete;
	WalkState& operator=(const WalkState& other) = delete;
};