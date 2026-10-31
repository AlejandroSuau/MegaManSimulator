#pragma once

#include "raylib.h"

class CharacterState;

enum class CharSpriteDirection
{
	State_IdleRight = 0,
	State_IdleLeft,
	State_WalkRight,
	State_WalkLeft,
	State_JumpRight,
	State_JumpLeft
};

struct FrameRecPos
{
	Rectangle frameRec;
	Vector2 frameOrigin;
	float durationInSeconds = 0.1;
};


class GameCharacter
{
public :
	GameCharacter();

	virtual ~GameCharacter() = default;

	virtual void InitGameCharacter();
	virtual void UpdateGameCharacter(float deltaTime);
	virtual void DrawGameCharacter();
	virtual void UnloadGameCharacter();

	virtual bool IsGrounded() const = 0;
	virtual bool IsMoving() const = 0;

	//SPRITES
	CharSpriteDirection GetCharSpriteState() const { return CharCurrentSpriteState; }
	void SetCharSpriteState(CharSpriteDirection newState) { CharCurrentSpriteState = newState; }

	Rectangle& GetPlayerFrameRec() { return frameRec; }
	void SetPlayerFrameRec(Rectangle newRec) { frameRec = newRec; }

	Vector2 GetPosition() const { return position_; }
	void SetPosition(Vector2 position) { position_ = position; }

	// State Mahines functions
	inline CharacterState* getCurrentState() const { return currentState; }
	// In here, we'll delegate the state transition to the currentState
	void updateState();
	// This will get called by the current state
	void setState(CharacterState& newState);

	int framesCounter = 0;
	int framesSpeed = 10;
	int currentFrame = 0;

protected:
	CharacterState* currentState = nullptr;

	//Srprite Anim
	Rectangle frameRec{};

	CharSpriteDirection CharCurrentSpriteState = CharSpriteDirection::State_IdleLeft;
	
	Vector2 position_{};
};


