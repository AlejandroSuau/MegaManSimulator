#pragma once

#include "raylib.h"

enum class SoundType
{
	IntroTheme = 0,
	GameOver,
	Death,
	EatingCherry,
	EatingGhost,
	EatingPowerPill,
	GameStart,
	Siren,
	Victory,
	WakaWaka
};

class AudioManager
{
public:
	AudioManager();

	void InitAudioFiles();
	void UnloadAudioFiles();

	//Functons For music 
	void PlayIntroMusic(bool activate);
	void UpdateIntroMusic();

	//Fuunctions for GPP Music
	void PlayGameOverMusic(bool activate);
	void UpdateGameOverMusic();

	void PlaySoundEffect(SoundType sound);

private:
	Music IntroMusic;
	Music GameOverMusic;

	Sound DeathSound;
	Sound EatingCherrySound;
	Sound EatingGhostSound;
	Sound EatingPowerPillSound;
	Sound GameStartSound;
	Sound SirenSound;
	Sound VictorySound;
	Sound WakaWakaSound;
};
