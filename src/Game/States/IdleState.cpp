#include "IdleState.h"

#include "WalkState.h"
#include "JumpState.h"

CharSpriteDirection IdleState::GetSpriteState(
	bool isFacingLeft,
	bool isShooting) const {

	if (isShooting) {
		return isFacingLeft
			? CharSpriteDirection::State_IdleShootLeft
			: CharSpriteDirection::State_IdleShootRight;
	}

	return isFacingLeft
		? CharSpriteDirection::State_IdleLeft
		: CharSpriteDirection::State_IdleRight;
}

void IdleState::updateState(GameCharacter* character)
{
    if (!character->IsGrounded()) {
        character->setState(JumpState::getInstance());
    } else if (character->IsMoving()) {
        character->setState(WalkState::getInstance());
    }
}

CharacterState& IdleState::getInstance()
{
	static IdleState singleton;
	return singleton;
}
