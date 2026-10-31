#include "WalkState.h"

#include "IdleState.h"
#include "JumpState.h"

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
