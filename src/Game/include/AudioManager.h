#pragma once
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"
#include <queue>

class AudioManager
{
private:
	sf::Music bgmPlayer;
	std::deque<std::unique_ptr<sf::Sound>> soundQueue;
	sf::SoundBuffer soundBuffer;
	float bgmVolume { 45.f };
	float soudnVolume { 40.f };

	eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& gameDispatcher;

public:
	AudioManager(eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher);

	void Init();
	void Update();
	void PlayBGM(std::string path);
	void PlaySoud(std::string path);
	void Clear();
	void ClearStopped();
};