#include "Dragon.h"

#include <algorithm>
#include <cmath>

void Dragon::Init(
    float x,
    float minSurfaceY,
    float maxSurfaceY,
    bool startAtBottom,
    Texture2D body,
    Texture2D horn)
{
    position_.x = x;

    minY_ = std::min(minSurfaceY, maxSurfaceY);
    maxY_ = std::max(minSurfaceY, maxSurfaceY);

    startAtBottom_ = startAtBottom;

    bodyTexture_ = body;
    hornTexture_ = horn;

    Reset();
}

void Dragon::Reset()
{
    phase_ = startAtBottom_ ? maxY_ - minY_ : 0.f;
    const float surfaceY = startAtBottom_ ? maxY_ : minY_;
    position_.y = surfaceY + kHeight * 0.5f - kSurfaceInset;
    hornsVisible_ = false;
}

void Dragon::UpdateGameCharacter(float deltaTime)
{
    const float distance = maxY_ - minY_;
    if (distance <= 0.f) { return; }

    phase_ = std::fmod(phase_ + kSpeed * deltaTime, 2.f * distance);
    const float offset = (phase_ <= distance) ? phase_ : 2.f * distance - phase_;
    position_.y = minY_ + offset + kHeight * 0.5f - kSurfaceInset;
}

Rectangle Dragon::GetBounds() const
{
    return {
        position_.x - kWidth * 0.5f,
        position_.y - kHeight * 0.5f + kSurfaceInset,
        kWidth,
        12.f
    };
}

std::array<Rectangle, 2> Dragon::GetHornBounds() const
{
    const Rectangle surface = GetBounds();

    return {
        Rectangle{
            surface.x + 8.f,
            surface.y - kHornSize,
            kHornSize,
            kHornSize
        },
        Rectangle{
            surface.x + surface.width - 8.f - kHornSize,
            surface.y - kHornSize,
            kHornSize,
            kHornSize
        }
    };
}

void Dragon::UpdateHorns(Vector2 playerPosition)
{
    const float dx = playerPosition.x - position_.x;
    const float dy = playerPosition.y - GetBounds().y;
    const float radius = hornsVisible_ ? 420.f : 350.f;
    hornsVisible_ = (dx * dx + dy * dy <= radius * radius);
}

bool Dragon::TouchesHorns(Rectangle playerBounds) const
{
    if (!hornsVisible_) { return false; }

    for (const auto& horn : GetHornBounds()) {
        if (CheckCollisionRecs(playerBounds, horn)) {
            return true;
        }
    }

    return false;
}

void Dragon::DrawGameCharacter()
{
    const Rectangle source{
        0.f,
        0.f,
        static_cast<float>(bodyTexture_.width),
        static_cast<float>(bodyTexture_.height)
    };

    DrawTexturePro(
        bodyTexture_,
        source,
        { position_.x, position_.y, kWidth, kHeight },
        { kWidth * 0.5f, kHeight * 0.5f },
        0.f,
        WHITE);

    if (hornsVisible_) {
        const Rectangle source{
            0.f,
            0.f,
            static_cast<float>(hornTexture_.width),
            static_cast<float>(hornTexture_.height) };

        for (const auto& horn : GetHornBounds()) {
            DrawTexturePro(hornTexture_, source, horn, { 0.f, 0.f }, 0.f, WHITE);
        }
    }
}