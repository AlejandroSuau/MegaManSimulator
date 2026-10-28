#include "IdleState.h"


void IdleState::enter(GameCharacter* character) 
{
	//character->SetCharSpriteState(CharSpriteDirection::State_Idle);
}

void IdleState::updateState(GameCharacter* character)
{
	// if (IsKeyPressed(KEY_LEFT))
	// {
		// character->setState(WalkForwardState::getInstance());

	// }
	// else if (IsKeyPressed(KEY_RIGHT))
	// {
		// character->setState(WalkBackwardsState::getInstance());
	// }

	// add more states

}

CharacterState& IdleState::getInstance()
{
	static IdleState singleton;
	return singleton;
}
