#pragma once

#include "Game/Character.h"
#include "Game/MegaMan.h"
#include "Game/GlobalGameDefines.h"
#include "Game/Managers/GameManager.h"
#include "States/IdleState.h"

#include <string>             // String manipulation functions: strrchr(), strcmp()
#include <algorithm>
#include <cmath>
#include <initializer_list>

#include "raylib.h"


void MegaMan::InitGameCharacter()
{
    GameCharacter::InitGameCharacter();

    // Create a little lambda as helper for load the animations
    const auto loadAnimation = [this](
        CharSpriteDirection sprite,
        const char* path,
        float width,
        float height,
        std::initializer_list<float> durations) {

            auto& animation = animations_[sprite];
            animation.texture = LoadTexture(path);
            int index = 0;
            for (auto duration : durations) {
                animation.frames.push_back(
                    FrameRecPos{
                    { index * width, 0.f, width, height },
                    { width * 0.5f, height },
                    duration});
                
                ++index;
            }
    };

    loadAnimation(
        CharSpriteDirection::State_IdleLeft,
        "resources/Game/MegaMan/IdleLeft.png",
        100.f, 100.f, {1.8f, 0.12f});

    loadAnimation(
        CharSpriteDirection::State_IdleRight,
        "resources/Game/MegaMan/IdleRight.png",
        100.f, 100.f, { 1.8f, 0.12f });

    loadAnimation(
        CharSpriteDirection::State_WalkRight,
        "resources/Game/MegaMan/WalkRight.png",
        150.0f, 100.0f, { 0.125f, 0.125f, 0.125f });

    loadAnimation(
        CharSpriteDirection::State_WalkLeft,
        "resources/Game/MegaMan/WalkLeft.png",
        150.0f, 100.0f, { 0.125f, 0.125f, 0.125f });

    loadAnimation(
        CharSpriteDirection::State_JumpRight,
        "resources/Game/MegaMan/JumpRight.png",
        150.0f, 150.0f, { 0.1f });

    loadAnimation(
        CharSpriteDirection::State_JumpLeft,
        "resources/Game/MegaMan/JumpLeft.png",
        150.0f, 150.0f, { 0.1f });

    Reset({ 0.0f, 0.0f });
}

Rectangle MegaMan::GetBounds() const
{
	return Rectangle{
		position_.x - kCollisionWidth * 0.5f,
		position_.y - kCollisionHeight * 0.5f,
        kCollisionWidth,
        kCollisionHeight
	};
}

void MegaMan::Reset(Vector2 position)
{
    SetPosition(position);

    currentVerticalSpeed_ = 0.0f;
    currentHorizontalSpeed_ = 0.0f;
    canJump_ = false;
    isFacingLeft_ = false;

    currentFrame = 0;
    animationTime_ = 0.0f;

    setState(IdleState::getInstance());
    SetCharSpriteState(CharSpriteDirection::State_IdleRight);

    SetPlayerFrameRec(
        animations_.at(GetCharSpriteState()).frames.front().frameRec);
}

void MegaMan::UpdateGameCharacter(float /*deltaTime*/)
{
    currentHorizontalSpeed_ = 0.0f;

    if (IsKeyDown(KEY_LEFT)) {
        currentHorizontalSpeed_ -= kHorizontalSpeed;
    }

    if (IsKeyDown(KEY_RIGHT)) {
        currentHorizontalSpeed_ += kHorizontalSpeed;
    }

    if (currentHorizontalSpeed_ != 0.0f) {
        isFacingLeft_ = currentHorizontalSpeed_ < 0.0f;
    }

    if (IsKeyPressed(KEY_SPACE) && canJump_) {
        currentVerticalSpeed_ = -kJumpSpeed;
        canJump_ = false;
    }
}

void MegaMan::UpdatePhysics(
    float deltaTime, 
    std::span<const Rectangle> platforms,
    float levelWidth)
{
    const float halfWidth = kCollisionWidth * 0.5f;
    const float halfHeight = kCollisionHeight * 0.5f;
    const float maxLevelX = std::max(halfWidth, levelWidth - halfWidth);

    const float previousBottom = position_.y + halfHeight;

    // Movement from input
    position_.x += currentHorizontalSpeed_ * deltaTime;
    position_.x = std::clamp(position_.x, halfWidth, maxLevelX);

    // Gravity modifies velocity and velocity position.
    currentVerticalSpeed_ += GameGlobalVar::gravity * deltaTime;
    position_.y += currentVerticalSpeed_ * deltaTime;

    const Rectangle bounds = GetBounds();
    const float currentBottom = bounds.y + bounds.height;

    bool isLanded = false;
    float landingY = currentBottom;

    for (const auto& platform : platforms) {
        const bool doesOverlapX =
            bounds.x + bounds.width > platform.x &&
            bounds.x < platform.x + platform.width;

        const bool doesCrossTop =
            previousBottom <= platform.y &&
            currentBottom >= platform.y;

        if (currentVerticalSpeed_ >= 0.0f &&
            doesOverlapX && doesCrossTop) {
            landingY = std::min(landingY, platform.y);
            isLanded = true;
        }
    }

    canJump_ = isLanded;

    if (isLanded) {
        position_.y = landingY - halfHeight;
        currentVerticalSpeed_ = 0.0f;
    }
}

void MegaMan::UpdateAnimation(float deltaTime)
{
    const auto nextSprite = currentState->GetSpriteState(isFacingLeft_);
    if (nextSprite != GetCharSpriteState()) {
        SetCharSpriteState(nextSprite);

        currentFrame = 0;
        animationTime_ = 0.0f;
    }

    const auto& animation = animations_.at(GetCharSpriteState());
    const int frameCount = static_cast<int>(animation.frames.size());
    if (frameCount > 1) {
        animationTime_ += deltaTime;

        // Advance multiple frames if needed to keep animation in sync with elapsed time.
        while (animationTime_ >= animation.frames.at(currentFrame).durationInSeconds) {
            animationTime_ -= animation.frames.at(currentFrame).durationInSeconds;

            currentFrame = (currentFrame + 1) % frameCount;
        }
    }

    SetPlayerFrameRec(animation.frames.at(currentFrame).frameRec);
}

void MegaMan::DrawGameCharacter()
{
    const auto& animation = animations_.at(GetCharSpriteState());
    const auto& frame = animation.frames.at(currentFrame);

    const Rectangle destination{
       position_.x,
       position_.y + kCollisionHeight * 0.5f,
       frame.frameRec.width,
       frame.frameRec.height
    };

    DrawTexturePro(animation.texture, frame.frameRec, destination, frame.frameOrigin, 0.0f, WHITE);
}

void MegaMan::UnloadGameCharacter()
{
    for (auto& [sprite, animation] : animations_) {
        UnloadTexture(animation.texture);
    }
}

// State Machines
void MegaMan::setState(CharacterState& newState)
{
    if (currentState) {
        // do something before we change state
        currentState->exit(this);
    }

	currentState = &newState;  // change state
	currentState->enter(this); // do something after we change state
}

void MegaMan::updateState()
{
	// Delegate the task of determining the next state to the current state!
	currentState->updateState(this);
}

