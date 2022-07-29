#include "GameMusic.h"

#include "Game.h"

GameMusic::GameMusic()
{
	Game::registeredMusic.emplace_back(this);
}

GameMusic::~GameMusic()
{
	auto elementIndex = std::find(Game::registeredMusic.begin(), Game::registeredMusic.end(), this);
	if (elementIndex != Game::registeredMusic.end())
	{
		Game::registeredMusic.erase(elementIndex);
	}
}

void GameMusic::SetMasterVolume(float masterVolume)
{
	_masterVolume = masterVolume;
	UpdateInternalVolume();
}

float GameMusic::GetMasterVolume()
{
	return _masterVolume;
}

void GameMusic::SetBaseVolume(float volume)
{
	_baseVolume = volume;
	UpdateInternalVolume();
}

float GameMusic::GetBaseVolume()
{
	return _baseVolume;
}

void GameMusic::UpdateInternalVolume()
{
	setVolume(_baseVolume * _masterVolume);
}
