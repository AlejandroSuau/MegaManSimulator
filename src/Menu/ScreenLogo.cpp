#include "ScreenLogo.h"

#include <algorithm>

namespace
{
    constexpr float FADE_IN_TIME = 1.0f;
    constexpr float DISPLAY_TIME = 1.0f;
    constexpr float FADE_OUT_TIME = 1.0f;
}

ScreenLogoState::ScreenLogoState()
{
}

ScreenLogoState& ScreenLogoState::getInstance()
{
    static ScreenLogoState singleton;
    return singleton;
}

//-------------------------------------------------------------
void ScreenLogoState::InitScreen(void)
{
    alpha_ = 1.0f;

    logo_ = LoadTexture("resources/Logo-TheTrueBooleanGames.png");
}

//-------------------------------------------------------------
void ScreenLogoState::UpdateScreen(float deltaTime)
{
    chargeTime_ += deltaTime;
    if (chargeTime_ < FADE_IN_TIME) {
        // Fade In
        alpha_ = chargeTime_ / FADE_IN_TIME;
    }
    else if (chargeTime_ < FADE_IN_TIME + DISPLAY_TIME) {
        // Showing N seconds
        alpha_ = 1.f;
    }
    else if (chargeTime_ < FADE_IN_TIME + DISPLAY_TIME + FADE_OUT_TIME) {
        // Fade Out
        const float fadeOutProgress = (chargeTime_ - FADE_IN_TIME - DISPLAY_TIME) / FADE_OUT_TIME;
        alpha_ = 1.f - fadeOutProgress;
    }
    else {
        // End
        alpha_ = 0.f;
        finishScreen_ = 1;
    }
}

//-------------------------------------------------------------

void ScreenLogoState::DrawScreen(void)
{
    // Scaling logo for aspect ratio
    const float screenWidth = static_cast<float>(GetScreenWidth());
    const float screenHeight = static_cast<float>(GetScreenHeight());

    const float scaleX = screenWidth / static_cast<float>(logo_.width);
    const float scaleY = screenHeight / static_cast<float>(logo_.height);

    const float scale = std::min(scaleX, scaleY);
    const float width = logo_.width * scale;

    const float height = logo_.height * scale;

    const float x = (screenWidth - width) / 2.0f;
    const float y = (screenHeight - height) / 2.0f;

    Rectangle srcRect{ 0.0f, 0.0f, static_cast<float>(logo_.width), static_cast<float>(logo_.height) };
    Rectangle dstRect{ x, y, width, height };
    DrawTexturePro(logo_, srcRect, dstRect, { 0.0f, 0.0f }, 0.0f, Fade(WHITE, alpha_));
}

//-------------------------------------------------------------
void ScreenLogoState::UnloadScreen(void)
{

}

//-------------------------------------------------------------
int  ScreenLogoState::FinishScreen(void)
{
    return finishScreen_;
}