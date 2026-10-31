#include "JumpState.h"

#include "IdleState.h"
#include "WalkState.h"

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
