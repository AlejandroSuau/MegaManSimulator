#pragma once

#include "Game/Character.h"

#include <vector>
#include <string>
#include <span>
#include <map>

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

	bool IsGrounded() const override { return canJump_; }
	bool IsMoving() const override { return currentHorizontalSpeed_ != 0.f;  }

	Rectangle GetBounds() const;

	bool CanJump() const { return canJump_; }
	void SetCanJump(bool canJump) { canJump_ = canJump; }

	void Reset(Vector2 position);
	void UpdatePhysics(
		float deltaTime,
		std::span<const Rectangle> platforms,
		float levelWidth);
	void UpdateAnimation(float deltaTime);
	void UpdateShooting(float deltaTime);

private:
	struct AnimationClip
	{
		Texture2D texture{};
		std::vector<FrameRecPos> frames;
	};

	struct Projectile
	{
		Vector2 position{};
		float velocityX = 0.f;
		float lifeTimeInSeconds = 0.f;
	};

	// Shooting
	static constexpr float kShotInterval = 1.f;
	static constexpr float kProjectileLifetime = 3.f;
	static constexpr float kProjectileSpeed = 600.f;
	static constexpr float kShootingPoseDuration = 0.2f;
	static constexpr float kProjectileRadius = 15.f;

	// Movement
	static constexpr float kJumpSpeed = 300.f;
	static constexpr float kHorizontalSpeed = 200.f;
	static constexpr float kCollisionWidth = 100.0f;
	static constexpr float kCollisionHeight = 100.0f;

	std::vector<Projectile> projectiles_;
	Texture2D projectileTexture_{};

	float currentVerticalSpeed_ = 0.f;
	float currentHorizontalSpeed_ = 0.0f;
	bool canJump_ = false;

	std::map<CharSpriteDirection, AnimationClip> animations_;

	float animationTime_ = 0.0f;
	bool isFacingLeft_ = false;

	float shotCooldown_ = 0.f;
	float shootingPoseTime_ = 0.f;
};
