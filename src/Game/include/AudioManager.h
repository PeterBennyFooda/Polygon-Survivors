#pragma once
#include "GlobalGameSettings.h"
#include <queue>

class AudioManager
{
private:
	sf::Music bgmPlayer;
	std::deque<sf::Sound*> soundQueue;
	sf::SoundBuffer soundBuffer;
	float bgmVolume { 50.f };
	float soudnVolume { 50.f };

public:
	void Update();
	void PlayBGM(std::string path);
	void PlaySoud(std::string path);
	void Clear();
	void ClearStopped();
};