#pragma once

#include "Game/Character.h"

class Copipi final : public GameCharacter
{
public:
    void Init(Vector2 spawn, Texture2D texture);
    void TryActivate(Vector2 playerPosition);
    void ResetPosition();

    void UpdateGameCharacter(float deltaTime) override;
    void DrawGameCharacter() override;

    bool IsGrounded() const override { return false; }
    bool IsMoving() const override { return IsAlive() && velocityX_ != 0.f; }

    bool IsAlive() const { return health_ > 0; }
    int GetHealth() const { return health_; }

    Rectangle GetBounds() const;

    void TakeHit();
    void Kill();

private:
    static constexpr float kSize = 64.f;
    static constexpr float kSpeed = 100.f;
    static constexpr float kActivationDistance = 600.f;
    static constexpr int kDefaultHealth = 3;

    Vector2 spawnPosition_{};

    // Textura compartida: ScreenGameplay se encarga de descargarla.
    Texture2D texture_{};

    float velocityX_ = 0.f;
    int health_ = kDefaultHealth;
    bool active_ = false;
};