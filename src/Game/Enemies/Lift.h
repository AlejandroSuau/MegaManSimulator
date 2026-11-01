#pragma once

#include "Game/Character.h"

class Lift final : public GameCharacter
{
public:
    void Init(
        float minX,
        float maxX,
        float surfaceY,
        bool startAtRight,
        Texture2D texture);

    void Reset();

    void UpdateGameCharacter(float deltaTime) override;
    void DrawGameCharacter() override;
    
    bool IsGrounded() const override { return false; }
    bool IsMoving() const override { return maxX_ > minX_; }
    
    float GetDisplacementX() const { return displacementX_; }
    Rectangle GetBounds() const;

private:
    static constexpr float kWidth = 150.f;
    static constexpr float kHeight = 105.f;
    static constexpr float kSurfaceInset = 10.f;
    static constexpr float kSpeed = 100.f;

    Texture2D texture_{};

    float minX_ = 0.f;
    float maxX_ = 0.f;

    float phase_ = 0.f;
    float displacementX_ = 0.f;

    bool startAtRight_ = false;
};