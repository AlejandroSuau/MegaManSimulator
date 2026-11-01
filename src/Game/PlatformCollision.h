#pragma once

#include "raylib.h"

// We need this to compare previous player position aswell as 
// platform previous position, so we can ensure that player did not
// went through it.

struct PlatformCollision
{
    Rectangle bounds{}; // current pos
    Vector2 displacement{}; // how much moved on that frame
};
