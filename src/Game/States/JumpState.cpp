#include "JumpState.h"

#include "IdleState.h"
#include "WalkState.h"

CharSpriteDirection JumpState::GetSpriteState(
	bool isFacingLeft,
	bool isShooting) const {

	if (isShooting) {
		return isFacingLeft
			? CharSpriteDirection::State_JumpShootLeft
			: CharSpriteDirection::State_JumpShootRight;
	}

	return isFacingLeft
		? CharSpriteDirection::State_JumpLeft
		: CharSpriteDirection::State_JumpRight;
}

void JumpState::updateState(GameCharacter* character)
{
    if (character->IsGrounded()) {
        if (character->IsMoving()) {
            character->setState(WalkState::getInstance());
        } else {
            character->setState(IdleState::getInstance());
        }
    }
}

CharacterState& JumpState::getInstance()
{
	static JumpState singleton;
	return singleton;
}
