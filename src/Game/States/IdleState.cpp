#include "IdleState.h"

#include "WalkState.h"
#include "JumpState.h"

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
