#pragma once

#include "Game/Character.h"
#include "Game/MegaMan.h"
#include "Game/GlobalGameDefines.h"
#include "Game/Managers/GameManager.h"
#include "States/IdleState.h"

#include <string>             // String manipulation functions: strrchr(), strcmp()

#include "raylib.h"


void MegaMan::InitGameCharacter()
{

	//Let's fill the vector MegaManSprtes Data
	
	//IDLE
	CharSprites_Counter[CharSpriteDirection::State_IdleRight] = 1;
	CharSprites_Idle_Right[0] = FrameRecPos{ {0,0,75,75}, {0.f, 0.f} };

	CharSprites_Counter[CharSpriteDirection::State_IdleLeft] = 1;
	CharSprites_Idle_Left[0] = FrameRecPos{ {75,0,75,75}, {0.f, 0.f} };

	//WALK FORWARD
	CharSprites_Counter[CharSpriteDirection::State_WalkForward] = 2;

	CharSprites_WalkForward[0] = FrameRecPos{ {0,0,75,75}, {0.f, 0.f} };
	CharSprites_WalkForward[1] = FrameRecPos{ {75,0,75,75}, {0.f, 0.f} };

	//init State
	currentState = &IdleState::getInstance();

	SetCharSpriteState(CharSpriteDirection::State_IdleRight);
	SetPlayerFrameRec(CharSprites_Idle_Right[0].frameRec);

}

void MegaMan::UpdateGameCharacter(float deltaTime)
{

	//Update State Machine
	updateState();

    // add here the updateframe logic to be able to see all the sprites from one state
}

void MegaMan::DrawGameCharacter()
{
}

void MegaMan::UnloadGameCharacter()
{

}

// State Machines
void MegaMan::setState(CharacterState& newState)
{
	currentState->exit(this);  // do something before we change state
	currentState = &newState;  // change state
	currentState->enter(this); // do something after we change state
}

void MegaMan::updateState()
{
	// Delegate the task of determining the next state to the current state!
	currentState->updateState(this);
}

