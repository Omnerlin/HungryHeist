#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Collider.h"
#include "SpriteAnimator.h"
#include "DrawnEntity.h"
#include "GameSound.h"

struct Player {
	// Transform
	EntityTransform transform;
	float floorOffset{ 0 };
	DrawnEntity<sf::Sprite> sprite;

	// Movement
	float groundAcceleration{ 800.f };
	float friction{ 50.f };
	float maxSpeedX{ 200.f };
	float maxSpeedY{ 1000.f };
	float gravity{ 800.f };
	float jumpForce{ 600.f };
	float airDrag{ 1000.f };
	bool grounded{ false };
	bool isJumping{ false };
	bool groundedLastFrame{ false };
	bool running{ false };
	bool facingLeft{ false };
	bool captured{ false };
	bool inputEnabled { true };
	sf::Vector2f velocity{ 0,0 };
	sf::Vector2f prevPosition{ 0,0 };

	void Initialize();
	void AddVelocity(const sf::Vector2f& addVelocity);
	void AddVelocity(float x, float y);
	void Update(float deltaTime);
	void LoadSettingsFromConfig();
	void ResolveMovementCollision(Collider* other);

	SpriteAnimator animator;
	SpriteAnimation idleAnim;
	SpriteAnimation runAnim;
	SpriteAnimation jumpAnim;
	SpriteAnimation sadAnim;

	GameSound jumpSound;
	GameSound landingSound;

	// Collision
	Collider collider;

	unsigned int GetWidth() const
	{
		return sprite.drawable.getTexture()->getSize().x;
	}
	unsigned int GetHeight() const
	{
		return sprite.drawable.getTexture()->getSize().y;
	}
	const sf::Vector2u& getSize() {
		return sprite.drawable.getTexture()->getSize();
	}
};

