#include "include/AudioManager.h"

AudioManager::AudioManager(eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher) :
	gameDispatcher(mDispatcher)
{
	Init();
}

void AudioManager::Init()
{
	gameDispatcher.appendListener(EventNames::BGMEvent, [this](const MyEvent& e) {
		PlayBGM(e.message);
	});

	gameDispatcher.appendListener(EventNames::SoundEvent, [this](const MyEvent& e) {
		PlaySoud(e.message);
	});
}

void AudioManager::Update()
{
	ClearStopped();
}

void AudioManager::PlayBGM(std::string path)
{
	if (!bgmPlayer.openFromFile(path))
	{
		//error..
	}
	else
	{
		bgmPlayer.setLoop(true);
		bgmPlayer.setVolume(bgmVolume);
		bgmPlayer.play();
	}
}

void AudioManager::PlaySoud(std::string path)
{
	sf::Sound* sound(new sf::Sound());
	std::unique_ptr<sf::Sound> uPtr { sound };

	if (!soundBuffer.loadFromFile(path))
	{
		//error...
	}
	else
	{
		sound->setBuffer(soundBuffer);
		soundQueue.emplace_back(std::move(uPtr));
		sound->play();
	}
}

void AudioManager::Clear()
{
	if (!soundQueue.empty())
		soundQueue.clear();
}

void AudioManager::ClearStopped()
{
	if (!soundQueue.empty())
	{
		soundQueue.erase(
			std::remove_if(std::begin(soundQueue), std::end(soundQueue), [](const std::unique_ptr<sf::Sound>& sound) {
				return sound->getStatus() == sf::SoundSource::Status::Stopped;
			}),
			std::end(soundQueue));
	}
}