#include "WalkState.h"

#include "IdleState.h"
#include "JumpState.h"

CharSpriteDirection WalkState::GetSpriteState(
	bool isFacingLeft,
	bool isShooting) const {

	if (isShooting) {
		return isFacingLeft
			? CharSpriteDirection::State_WalkShootLeft
			: CharSpriteDirection::State_WalkShootRight;
	}

	return isFacingLeft
		? CharSpriteDirection::State_WalkLeft
		: CharSpriteDirection::State_WalkRight;
}

void WalkState::updateState(GameCharacter* character)
{
    if (!character->IsGrounded()) {
        character->setState(JumpState::getInstance());
    } else if (!character->IsMoving()) {
        character->setState(IdleState::getInstance());
    }
}

CharacterState& WalkState::getInstance()
{
	static WalkState singleton;
	return singleton;
}
