#include "Copipi.h"

void Copipi::Init(Vector2 spawn, Texture2D texture)
{
    spawnPosition_ = spawn;
    texture_ = texture;
    health_ = kDefaultHealth;

    ResetPosition();
}

void Copipi::ResetPosition()
{
    position_ = spawnPosition_;
    velocityX_ = 0.f;
    active_ = false;
}

void Copipi::TryActivate(Vector2 playerPosition)
{
    if (!IsAlive() || active_) { return; }

    const float dx = playerPosition.x - position_.x;
    const float dy = playerPosition.y - position_.y;

    if ((dx * dx + dy * dy) <= kActivationDistance * kActivationDistance) {
        active_ = true;
        velocityX_ = dx < 0.f ? -kSpeed : kSpeed;
    }
}

void Copipi::UpdateGameCharacter(float deltaTime)
{
    if (IsAlive()) {
        position_.x += velocityX_ * deltaTime;
    }
}

Rectangle Copipi::GetBounds() const
{
    return {
        position_.x - kSize * 0.5f,
        position_.y - kSize * 0.5f,
        kSize,
        kSize
    };
}

void Copipi::TakeHit()
{
    if (!IsAlive()) { return; }

    --health_;
    if (!IsAlive()) {
        Kill();
    }
}

void Copipi::Kill()
{
    health_ = 0;
    velocityX_ = 0.f;
    active_ = false;
}

void Copipi::DrawGameCharacter()
{
    if (!IsAlive()) { return; }

    const Rectangle source{
        0.f,
        0.f,
        static_cast<float>(texture_.width) * (velocityX_ > 0.f ? -1.f : 1.f),
        static_cast<float>(texture_.height) };

    DrawTexturePro(texture_, source, GetBounds(), { 0.f, 0.f }, 0.f, WHITE);
}