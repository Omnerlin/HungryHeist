#pragma once
#include <Typedefs.h>
#include <string>
#include <vector>

struct SpriteAnimation {
	std::string name;
	std::string texture;
	int32 frameWidth{ 32 };
	int32 frameHeight{ 32 };
	int32 startFrame{ 0 };
	int32 endFrame{ 0 };
	float originX{ 0.5f };
	float originY{ 1.0f };
	float duration{ 1 };
	bool loop{ true };
};