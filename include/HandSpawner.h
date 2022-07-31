#pragma once
#include <random>
#include "Hand.h"

struct HandSpawner
{
	// Collection of hands
	std::vector<Hand> hands;

	// Positions that hands will spawn from
	std::vector<HandSpawnPosition> handPositions;

	// Hand positions that are free for a hand spawn
	std::vector<HandSpawnPosition> inactiveHandPositions;

	// Random distributions
	std::uniform_int_distribution<int> colorDistribution{ 0, 4 };
	std::uniform_int_distribution<int> sideDistribution{ 0, 1 };
	std::uniform_int_distribution<int> topDistribution{ 2, 3 };

	float timeBetweenHands = { 1.f };
	float timeSinceLastHand{ 0 };
	bool handSpawningEnabled { true };

	void Initialize();
	void ResetHands();
	void Update(float deltaTime);
};
