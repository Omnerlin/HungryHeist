#pragma once
#include "PerlinNoise.hpp"

struct Noise
{
public:
	static const siv::BasicPerlinNoise<float> Instance;
private:
	static siv::BasicPerlinNoise<float>::seed_type seed;
};
