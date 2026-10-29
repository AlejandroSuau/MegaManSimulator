#pragma once

#include "raylib.h"

#include <map>

class CharacterState;

enum class CharSpriteDirection
{
	State_IdleRight = 0,
	State_IdleLeft,
	State_WalkForward

};

struct FrameRecPos
{
	Rectangle frameRec;
	Vector2   frameOrigin;
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

	//SPRITES
	CharSpriteDirection GetCharSpriteState() const { return CharCurrentSpriteState; }
	void SetCharSpriteState(CharSpriteDirection newState) { CharCurrentSpriteState = newState; }

	Rectangle& GetPlayerFrameRec() { return frameRec; }
	void SetPlayerFrameRec(Rectangle newRec) { frameRec = newRec; }

	Vector2 GetPosition() const { return position_; }
	void SetPosition(Vector2 position) { position_ = position; }

protected:

	std::map<CharSpriteDirection, int> CharSprites_Counter;

	std::map<int, FrameRecPos> CharSprites_Idle_Right;
	std::map<int, FrameRecPos> CharSprites_Idle_Left;
	std::map<int, FrameRecPos> CharSprites_WalkForward;
	
	Vector2 position_{};

public:	
	int framesCounter = 0;
	int framesSpeed = 10;
	int currentFrame = 0;

	// State Mahines functions
	inline CharacterState* getCurrentState() const { return currentState; }
	// In here, we'll delegate the state transition to the currentState
	void updateState();
	// This will get called by the current state
	void setState(CharacterState& newState);

protected:
		
	CharacterState* currentState = nullptr;

	//Srprite Anim
	Rectangle frameRec;

	CharSpriteDirection CharCurrentSpriteState;
};


