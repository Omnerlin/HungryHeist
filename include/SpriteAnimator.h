#pragma once
#include <iostream>

#include "SpriteAnimation.h"
#include "SFML/Graphics.hpp"
#include "unordered_map"
#include "DrawnEntity.h"

struct SpriteAnimator
{
	DrawnEntity<sf::Sprite>* targetSprite{ nullptr };
	std::unordered_map<std::string, SpriteAnimation> spriteAnimations;
	float secondsSinceAnimStart = 0;
	bool shouldUpdate{ true };
	bool reset{ false };
	SpriteAnimation* currentAnimation{ nullptr };

	void SetState(const std::string& state)
	{
		const auto& anim = spriteAnimations.find(state);
		if (anim == spriteAnimations.end())
		{
			std::cout << "Could not find state " << state << std::endl;
			return;
		}
		if (&(anim->second) == currentAnimation)
		{
			return;
		}

		currentAnimation = &spriteAnimations[state];
		secondsSinceAnimStart = 0;
		reset = true;
	}

	void Update(float deltaTime)
	{
		if (currentAnimation == nullptr) return;
		if (reset) {
			reset = false;
		}
		else {
			if (secondsSinceAnimStart < currentAnimation->duration)
			{
				secondsSinceAnimStart += deltaTime;
			}
			if (secondsSinceAnimStart > currentAnimation->duration && currentAnimation->loop)
			{
				secondsSinceAnimStart -= currentAnimation->duration;
			}
		}

		int totalFrames = (currentAnimation->endFrame - currentAnimation->startFrame) + 1;

		if (targetSprite == nullptr) return;
		int currentFrame = currentAnimation->startFrame + static_cast<int>((secondsSinceAnimStart / currentAnimation->duration) * totalFrames);
		targetSprite->drawable.setTextureRect(sf::IntRect(currentAnimation->frameWidth * currentFrame, 0,
			currentAnimation->frameWidth, currentAnimation->frameHeight));
		targetSprite->transform.SetOrigin(currentAnimation->originX * currentAnimation->frameWidth, currentAnimation->originY * currentAnimation->frameHeight);
	}

	void AddAnimation(SpriteAnimation& animation)
	{
		if (spriteAnimations.find(animation.name) != spriteAnimations.end())
		{
			std::cout << "Tried to add anim state " << animation.name << "but we already have a state by that name." << std::endl;
			return;
		}
		spriteAnimations.emplace(animation.name, animation);
	}
};
