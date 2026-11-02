#pragma once

#include "Game/Character.h"

#include <array>

class Dragon final : public GameCharacter
{
public:
    void Init(
        float x,
        float minSurfaceY,
        float maxSurfaceY,
        bool startAtBottom,
        Texture2D body,
        Texture2D horn);

    void Reset();

    void UpdateGameCharacter(float deltaTime) override;
    void DrawGameCharacter() override;

    bool IsGrounded() const override { return false; }
    bool IsMoving() const override { return maxY_ > minY_; }

    Rectangle GetBounds() const;

    std::array<Rectangle, 2> GetHornBounds() const;

    void UpdateHorns(Vector2 playerPosition);

    bool AreHornsVisible() const { return hornsVisible_; }
    bool TouchesHorns(Rectangle playerBounds) const;

private:
    static constexpr float kWidth = 258.f;
    static constexpr float kHeight = 288.f;
    static constexpr float kSurfaceInset = 8.f;

    static constexpr float kSpeed = 60.f;
    static constexpr float kHornSize = 50.f;

    Texture2D bodyTexture_{};
    Texture2D hornTexture_{};

    float minY_ = 0.f;
    float maxY_ = 0.f;
    float phase_ = 0.f;

    bool startAtBottom_ = false;
    bool hornsVisible_ = false;
};