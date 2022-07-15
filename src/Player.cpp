#include <Player.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>

#include "Physics.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif // _WIN32
#include "Input.h"
#include "Game.h"
#include "Assets.h"

void Player::LoadSettingsFromConfig() {
	char tmp[256];
	getcwd(tmp, 256);
	std::cout << "Current working directory: " << tmp << std::endl;
	std::ifstream config("config/player.json");
	std::stringstream configBuffer;
	configBuffer << config.rdbuf();

	try {
		nlohmann::json json;
		json = nlohmann::json::parse(configBuffer.str());
		float collisionSizeX = json["playerCollisionSizeX"].get<float>();
		float collisionSizeY = json["playerCollisionSizeY"].get<float>();
		collider.drawable.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
		collider.transform.SetOrigin(collider.drawable.getSize().x / 2,
			collider.drawable.getSize().y);
		sprite.transform.SetOrigin(sprite.drawable.getTexture()->getSize().x / 2.f, sprite.drawable.getTexture()->getSize().y);
		groundAcceleration = json["playerGroundAcceleration"].get<float>();
		friction = json["playerFriction"].get<float>();
		maxSpeedX = json["playerMaxSpeedX"].get<float>();
		maxSpeedY = json["playerMaxSpeedY"].get<float>();
		gravity = json["playerGravity"].get<float>();
		jumpForce = json["playerJumpForce"].get<float>();
		airDrag = json["playerAirDrag"].get<float>();
		floorOffset = json["playerFloorOffset"].get<float>();
	}
	catch (const std::exception& e) {
		std::cout << "Error opening config file." << std::endl;
		std::cerr << e.what() << std::endl;
	}
}


void Player::Update(float deltaTime) {

	running = false;

	if (captured)
	{
		return;
	}

	if(inputEnabled)
	{
		// Held Keys
		if (Input::KeyIsDown(KeyCode::A)) {
			running = true;
			if (grounded && velocity.x > 0) velocity.x = 0;
			AddVelocity(-sf::Vector2f((deltaTime * groundAcceleration), 0.f));
			facingLeft = false;
		}
		if (Input::KeyIsDown(KeyCode::D)) {
			if (grounded && velocity.x < 0) velocity.x = 0;
			running = true;
			AddVelocity(sf::Vector2f((deltaTime * groundAcceleration), 0.f));
			facingLeft = true;
		}
		if (Input::KeyWasPressed(KeyCode::Space) && grounded) {
			AddVelocity(0, -jumpForce);
		}
	}

	if (!grounded)
	{
		animator.SetState("Jump");
	}
	else
	{
		if (!running)
		{
			animator.SetState("Idle");
		}
		else
		{
			animator.SetState("Run");
		}
	}

	// Update current Position
	sf::Vector2f currentPosition = transform.GetWorldPosition();
	prevPosition = currentPosition;
	AddVelocity(0, gravity * deltaTime);
	currentPosition += velocity * deltaTime;

	if (!running) {
		if (velocity.x > 0) {
			AddVelocity((grounded ? -friction : -airDrag) * deltaTime, 0);
			if (velocity.x <= 0) {
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0) {
			AddVelocity((grounded ? friction : airDrag) * deltaTime, 0);
			if (velocity.x >= 0) {
				velocity.x = 0;
			}
		}
	}

	transform.SetWorldScale(facingLeft ? -1 : 1, 1);
	transform.SetWorldPosition(currentPosition);

	grounded = false;
	if (transform.GetWorldPosition().y >= floorOffset) {
		transform.SetWorldPosition(transform.GetWorldPosition().x, floorOffset);
		velocity.y = 0;
		grounded = true;
	}
}

void Player::Initialize()
{
	// Set up animations
	idleAnim.startFrame = 0;
	idleAnim.endFrame = 4;
	idleAnim.duration = 0.5f;
	idleAnim.name = "Idle";

	runAnim.startFrame = 5;
	runAnim.endFrame = 9;
	runAnim.duration = 0.4f;
	runAnim.name = "Run";

	jumpAnim.startFrame = 5;
	jumpAnim.endFrame = 7;
	jumpAnim.duration = 0.25f;
	jumpAnim.name = "Jump";
	jumpAnim.loop = false;

	sadAnim.startFrame = 10;
	sadAnim.endFrame = 11;
	sadAnim.duration = 1;
	sadAnim.name = "Sad";
	sadAnim.originY = 0.5f;

	animator.AddAnimation(idleAnim);
	animator.AddAnimation(runAnim);
	animator.AddAnimation(jumpAnim);
	animator.AddAnimation(sadAnim);

	// Set up child sprite transform
	sprite.transform.SetParent(&transform);
	sprite.transform.SetLocalPosition(0, 0);
	animator.targetSprite = &sprite;

	animator.SetState("Idle");
	sprite.drawable.setTexture(Assets::LoadTexture("assets/textures/SphynxAnims.png"));
	sprite.transform.SetOrigin(32 / 2, sprite.drawable.getTexture()->getSize().y);
	Physics::RegisterCollider(&collider);
	collider.CollisionStayCallback.emplace_back([this](Collider* col) { ResolveMovementCollision(col); });
	collider.transform.SetParent(&transform);
}

void Player::AddVelocity(const sf::Vector2f& addVelocity) {
	velocity.x = std::clamp(velocity.x + addVelocity.x, -maxSpeedX, maxSpeedX);
	velocity.y = std::clamp(velocity.y + addVelocity.y, -maxSpeedY, maxSpeedY);
}

void Player::ResolveMovementCollision(Collider* other) {
	// Test collision for player
	sf::FloatRect bounds = other->drawable.getGlobalBounds();
	if (other->HasCollisionDirectionEnabled(Bottom) && velocity.y < 0 &&
		prevPosition.y >
		(bounds.top + bounds.height + collider.drawable.getSize().y)) {
		// We hit the bottom
		velocity.y = 0;
		transform.SetWorldPosition(transform.GetWorldPosition().x, bounds.top + bounds.height +
			collider.drawable.getSize().y);
	}
	else if (other->HasCollisionDirectionEnabled(Top) && velocity.y > 0 &&
		prevPosition.y <= (bounds.top)) {
		// We hit the top
		grounded = true;
		velocity.y = 0;
		transform.SetWorldPosition(transform.GetWorldPosition().x, bounds.top);
	}
	else if (other->HasCollisionDirectionEnabled(Left) && velocity.x > 0 &&
		(prevPosition.x + collider.drawable.getSize().x / 2) <=
		bounds.left) {
		// We hit the left side
		velocity.x = 0;
		transform.SetWorldPosition(bounds.left - collider.drawable.getSize().x / 2,
			transform.GetWorldPosition().y);
	}
	else if (other->HasCollisionDirectionEnabled(Right) && velocity.x < 0 &&
		(prevPosition.x - collider.drawable.getSize().x / 2) >=
		bounds.left + bounds.width) {
		// We hit the right side
		velocity.x = 0;
		transform.SetWorldPosition(bounds.left + bounds.width +
			collider.drawable.getSize().x / 2,
			transform.GetWorldPosition().y);
	}
}

void Player::AddVelocity(float x, float y) { AddVelocity(sf::Vector2f(x, y)); }
