#include "Lift.h"

#include <algorithm>
#include <cmath>

void Lift::Init(
    float minX,
    float maxX,
    float surfaceY,
    bool startAtRight,
    Texture2D texture)
{
    minX_ = std::min(minX, maxX);
    maxX_ = std::max(minX, maxX);

    startAtRight_ = startAtRight;
    texture_ = texture;

    // Center of the sprite
    position_.y = surfaceY + kHeight * 0.5f - kSurfaceInset;

    Reset();
}

void Lift::Reset()
{
    const float distance = maxX_ - minX_;

    phase_ = startAtRight_ ? distance : 0.f;
    position_.x = startAtRight_ ? maxX_ : minX_;

    displacementX_ = 0.f;
}

void Lift::UpdateGameCharacter(float deltaTime)
{
    displacementX_ = 0.f;

    const float distance = maxX_ - minX_;
    if (distance <= 0.f) { return; }

    const float previousX = position_.x;
    
    // Fmod to allow repeat the movement (even if it is left between frames)
    phase_ = std::fmod(phase_ + kSpeed * deltaTime, 2.f * distance);
    const float offset = (phase_ <= distance) ? phase_ : 2.f * distance - phase_;
    
    position_.x = minX_ + offset;
    displacementX_ = position_.x - previousX;
}

Rectangle Lift::GetBounds() const
{
    return {
        position_.x - kWidth * 0.5f,
        position_.y - kHeight * 0.5f + kSurfaceInset,
        kWidth,
        12.f
    };
}

void Lift::DrawGameCharacter()
{
    const Rectangle source{
        0.f,
        0.f,
        static_cast<float>(texture_.width),
        static_cast<float>(texture_.height)
    };

    const Rectangle destination{ position_.x, position_.y, kWidth, kHeight};
    DrawTexturePro(
        texture_,
        source,
        destination,
        { kWidth * 0.5f, kHeight * 0.5f },
        0.f,
        WHITE);
}