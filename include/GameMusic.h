#pragma once
#include <SFML/Audio/Music.hpp>

struct GameMusic : sf::Music
{
public:
	GameMusic();
	~GameMusic();
	bool pauseWhenGamePauses{ true };
	void SetMasterVolume(float masterVolume); // 0 min, 1 max
	float GetMasterVolume();
	void SetBaseVolume(float volume); // 0 min, 100 max
	float GetBaseVolume();
	void UpdateInternalVolume();

private:
	float _baseVolume{ 100 };
	float _masterVolume{ 100 };
};
