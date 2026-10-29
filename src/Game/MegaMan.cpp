#pragma once

#include "Game/Character.h"
#include "Game/MegaMan.h"
#include "Game/GlobalGameDefines.h"
#include "Game/Managers/GameManager.h"
#include "States/IdleState.h"

#include <string>             // String manipulation functions: strrchr(), strcmp()
#include <algorithm>
#include <cmath>

#include "raylib.h"


void MegaMan::InitGameCharacter()
{

	//Let's fill the vector MegaManSprtes Data
    idleRightTexture_ = LoadTexture("resources/Game/MegaMan/IdleRight.png");
    idleLeftTexture_ = LoadTexture("resources/Game/MegaMan/IdleLeft.png");
	
    CharSprites_Counter.clear();
    CharSprites_Idle_Right.clear();
    CharSprites_Idle_Left.clear();

    CharSprites_Counter[CharSpriteDirection::State_IdleRight] = 2;
    CharSprites_Counter[CharSpriteDirection::State_IdleLeft] = 2;

    for (int i = 0; i < 2; ++i)
    {
        const FrameRecPos frame{
            { static_cast<float>(i) * 100.0f, 0.0f, 100.0f, 100.0f },
            { 50.0f, 100.0f }
        };

        CharSprites_Idle_Right[i] = frame;
        CharSprites_Idle_Left[i] = frame;
    }

    idleAnimationTime_ = 0.0f;

    SetCharSpriteState(CharSpriteDirection::State_IdleRight);
    SetPlayerFrameRec(CharSprites_Idle_Right.at(0).frameRec);

    currentState = &IdleState::getInstance();
    currentState->enter(this);
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

    SetCharSpriteState(CharSpriteDirection::State_IdleRight);

    currentFrame = 0;
    idleAnimationTime_ = 0.0f;

    SetPlayerFrameRec(CharSprites_Idle_Right.at(0).frameRec);
}

void MegaMan::UpdateGameCharacter(float /*deltaTime*/)
{
	updateState();

    currentHorizontalSpeed_ = 0.0f;

	if (IsKeyDown(KEY_LEFT))
	{
        currentHorizontalSpeed_ -= kHorizontalSpeed;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
        currentHorizontalSpeed_ += kHorizontalSpeed;
	}

	if (IsKeyPressed(KEY_SPACE) && canJump_)
	{
        currentVerticalSpeed_ = -kJumpSpeed;
        canJump_ = false;
	}

    // Update player sprite. Only if is moving (so we c an preserv last side state)
    const auto nextSprite = (currentHorizontalSpeed_ < 0.0f)
        ? CharSpriteDirection::State_IdleLeft
        : CharSpriteDirection::State_IdleRight;
    if (currentHorizontalSpeed_ != 0.f && nextSprite != GetCharSpriteState())
    {
        SetCharSpriteState(nextSprite);

        currentFrame = 0;
        idleAnimationTime_ = 0.0f;
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
    // Frame 0: open eyes. Frame 1: closed eyes.
    constexpr float frameDurations[] = { 1.8f, 0.12f };
    const bool isResting = canJump_ && currentHorizontalSpeed_ == 0.0f;

    if (!isResting) {
        currentFrame = 0;
        idleAnimationTime_ = 0.0f;
    } else {
        idleAnimationTime_ += deltaTime;

        const int frameCount = CharSprites_Counter.at(GetCharSpriteState());
        while (idleAnimationTime_ >= frameDurations[currentFrame])
        {
            idleAnimationTime_ -= frameDurations[currentFrame];
            currentFrame = (currentFrame + 1) % frameCount;
        }
    }

    const auto& frames = GetCharSpriteState() == CharSpriteDirection::State_IdleLeft
        ? CharSprites_Idle_Left
        : CharSprites_Idle_Right;

    SetPlayerFrameRec(frames.at(currentFrame).frameRec);
}

void MegaMan::DrawGameCharacter()
{
    const bool isFacingLeft = GetCharSpriteState() == CharSpriteDirection::State_IdleLeft;
    const auto& texture = isFacingLeft ? idleLeftTexture_ : idleRightTexture_;

    const auto& frames = isFacingLeft ? CharSprites_Idle_Left : CharSprites_Idle_Right;
    const auto& frame = frames.at(currentFrame);
    const Rectangle destination{
        position_.x,
        position_.y + kCollisionHeight * 0.5f,
        frame.frameRec.width,
        frame.frameRec.height
    };

    DrawTexturePro(texture,frame.frameRec, destination, frame.frameOrigin, 0.0f, WHITE);
}

void MegaMan::UnloadGameCharacter()
{
    UnloadTexture(idleRightTexture_);
    UnloadTexture(idleLeftTexture_);
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

