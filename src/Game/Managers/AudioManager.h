#pragma once

#include "raylib.h"

#include <array>

enum class SoundType
{
    Shoot,
    Land,
    EnemyHit,
    Death,
    Count
};

enum class MusicTrack
{
    Title,
    Stage,
    Victory,
    GameOver,
    Count
};

class AudioManager
{
public:
    void InitAudioFiles();
    void UnloadAudioFiles();

    void Update();

    void PlaySoundEffect(SoundType type);
    void PlayMusic(MusicTrack track);
    void StopMusic();

private:
    // In case we add more sounds/music. Handle it dynamically through Count
    std::array<Sound, static_cast<std::size_t>(SoundType::Count)> sounds_{};
    std::array<Music, static_cast<std::size_t>(MusicTrack::Count)> music_{};

    int currentTrack_ = -1;
};