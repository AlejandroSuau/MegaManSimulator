#pragma once

#include <map>
#include <string>
#include <span>

#include "Game/Character.h"


class MegaMan : public GameCharacter
{
public:

	virtual void InitGameCharacter() override;
	virtual void UpdateGameCharacter(float deltaTime) override;
	virtual void DrawGameCharacter() override;
	virtual void UnloadGameCharacter() override;

	//State Machine
	virtual void updateState();
	// This will get called by the current state
	virtual void setState(CharacterState& newState);

	Rectangle GetBounds() const;

	bool CanJump() const { return canJump_; }
	void SetCanJump(bool canJump) { canJump_ = canJump; }

	void Reset(Vector2 position);
	void UpdatePhysics(
		float deltaTime,
		std::span<const Rectangle> platforms,
		float levelWidth);
	void UpdateAnimation(float deltaTime);

private:
	static constexpr float kJumpSpeed = 300.f;
	static constexpr float kHorizontalSpeed = 200.f;
	static constexpr float kCollisionWidth = 100.0f;
	static constexpr float kCollisionHeight = 100.0f;

	float currentVerticalSpeed_ = 0.f;
	float currentHorizontalSpeed_ = 0.0f;
	bool canJump_ = false;

	Texture2D idleRightTexture_{};
	Texture2D idleLeftTexture_{};

	float idleAnimationTime_ = 0.0f;
};
