#include "GameSound.h"

#include "Game.h"

GameSound::GameSound()
{
	
	//Game::registeredSounds.emplace_back(this);
}

GameSound::~GameSound()
{
	//auto elementIndex = std::find(Game::registeredSounds.begin(), Game::registeredSounds.end(), this);
	//if (elementIndex != Game::registeredSounds.end())
	//{
	//	Game::registeredSounds.erase(elementIndex);
	//}
}

void GameSound::SetMasterVolume(float masterVolume)
{
	_masterVolume = masterVolume;
	UpdateInternalVolume();
}

float GameSound::GetMasterVolume()
{
	return _masterVolume;
}

void GameSound::SetBaseVolume(float volume)
{
	_baseVolume = volume;
	UpdateInternalVolume();
}

float GameSound::GetBaseVolume()
{
	return _baseVolume;
}

void GameSound::UpdateInternalVolume()
{
	setVolume(_baseVolume * _masterVolume);
}
