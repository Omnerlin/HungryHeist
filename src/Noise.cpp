#include "Noise.h"

siv::BasicPerlinNoise<float>::seed_type Noise::seed = 123456u;
const siv::BasicPerlinNoise<float> Noise::Instance = siv::BasicPerlinNoise<float>(seed);