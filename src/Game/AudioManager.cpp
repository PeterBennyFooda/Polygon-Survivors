#include "include/AudioManager.h"

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

	if (!soundBuffer.loadFromFile(path))
	{
		//error...
	}
	else
	{
		sound->setBuffer(soundBuffer);
		soundQueue.emplace_back(sound);
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
			std::remove_if(std::begin(soundQueue), std::end(soundQueue), [](const sf::Sound* sound) {
				return sound->getStatus() == sf::SoundSource::Status::Stopped;
			}),
			std::end(soundQueue));
	}
}