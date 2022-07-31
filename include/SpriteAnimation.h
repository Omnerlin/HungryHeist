#pragma once
#include <string>
#include <vector>

struct SpriteAnimation {
	std::string name;
	std::string texture;
	int frameWidth{ 32 };
	int frameHeight{ 32 };
	int startFrame{ 0 };
	int endFrame{ 0 };
	float originX{ 0.5f };
	float originY{ 1.0f };
	float duration{ 1 };
	bool loop{ true };
};