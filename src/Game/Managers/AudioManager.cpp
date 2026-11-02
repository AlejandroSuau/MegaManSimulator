#include "AudioManager.h"

// We assume all path, sounds and music are correct. So no defensive checks

void AudioManager::InitAudioFiles()
{
    if (!IsAudioDeviceReady()) { return; }

    UnloadAudioFiles();

    constexpr std::array soundPaths{
        "resources/Audio/Sounds/buster.wav",
        "resources/Audio/Sounds/land.wav",
        "resources/Audio/Sounds/enemy_hit.wav",
        "resources/Audio/Sounds/death.wav"
    };

    constexpr std::array musicPaths{
        "resources/Audio/Music/title.mp3",
        "resources/Audio/Music/air_man.mp3",
        "resources/Audio/Music/stage_clear.mp3",
        "resources/Audio/Music/game_over.mp3"
    };

    // Load sounds
    for (std::size_t i = 0; i < sounds_.size(); ++i) {
        sounds_[i] = LoadSound(soundPaths[i]);
        SetSoundVolume(sounds_[i], 0.65f);
    }

    // Load music
    for (std::size_t i = 0; i < music_.size(); ++i) {
        music_[i] = LoadMusicStream(musicPaths[i]);
        music_[i].looping =
            i == static_cast<std::size_t>(MusicTrack::Title) ||
            i == static_cast<std::size_t>(MusicTrack::Stage);

        SetMusicVolume(music_[i], 0.35f);
    }
}

void AudioManager::PlaySoundEffect(SoundType type)
{
    const auto index = static_cast<std::size_t>(type);
    PlaySound(sounds_[index]);
}

void AudioManager::PlayMusic(MusicTrack track)
{
    const int index = static_cast<int>(track);
    if (currentTrack_ == index) { return; }

    StopMusic();

    currentTrack_ = index;
    PlayMusicStream(music_[index]);
}

void AudioManager::StopMusic()
{
    if (currentTrack_ >= 0) {
        StopMusicStream(music_[currentTrack_]);
        currentTrack_ = -1;
    }
}

void AudioManager::Update()
{
    if (currentTrack_ >= 0) {
        UpdateMusicStream(music_[currentTrack_]);
    }
}

void AudioManager::UnloadAudioFiles()
{
    StopMusic();

    for (auto& sound : sounds_) {
       UnloadSound(sound);
    }

    for (auto& music : music_) {
        UnloadMusicStream(music);
    }
}