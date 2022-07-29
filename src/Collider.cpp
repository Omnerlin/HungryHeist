#include <Collider.h>
#include <Player.h>

#include "Assets.h"

bool Collider::HasCollisionDirectionEnabled(CollisionDirection direction) {
	return (direction | collisionDirection) == collisionDirection;
}

void Collider::InvokeInitialCallbackAgainstCollider(Collider* other) {
	if (colliderType == ColliderType::Solid &&
		other->colliderType == ColliderType::Solid) {
		other->InvokeCallbacks(other->CollisionBeginCallbacks, this);
		InvokeCallbacks(CollisionBeginCallbacks, other);
	}
	else {
		// do the same for triggers
		if (colliderType == ColliderType::Trigger) {
			other->InvokeCallbacks(other->TriggerOverlapBeginCallback, this);
		}
		if (other->colliderType == ColliderType::Trigger) {
			InvokeCallbacks(TriggerOverlapBeginCallback, other);
		}
	}
}

void Collider::InvokeStayCallbackAgainstCollider(Collider* other) {
	if (colliderType == ColliderType::Solid &&
		other->colliderType == ColliderType::Solid) {
		other->InvokeCallbacks(CollisionStayCallback, this);
		InvokeCallbacks(CollisionStayCallback, other);
	}
	else {
		// do the same for triggers
		if (colliderType == ColliderType::Trigger) {
			other->InvokeCallbacks(TriggerOverlapStayCallback, this);
		}
		if (other->colliderType == ColliderType::Trigger) {
			InvokeCallbacks(TriggerOverlapStayCallback, other);
		}
	}
}

void Collider::InvokeEndCallbackAgainstCollider(Collider* other) {
	if (colliderType == ColliderType::Solid &&
		other->colliderType == ColliderType::Solid) {
		other->InvokeCallbacks(CollisionEndCallback, this);
		InvokeCallbacks(CollisionEndCallback, other);
	}
	else {
		// do the same for triggers
		if (colliderType == ColliderType::Trigger) {
			other->InvokeCallbacks(other->TriggerOverlapEndCallback, this);
		}
		if (other->colliderType == ColliderType::Trigger) {
			InvokeCallbacks(TriggerOverlapEndCallback, this);
		}
	}
}

void Collider::InvokeCallbacks(const std::vector<std::function<void(Collider*)>>& callbacks, Collider* arg) {
	for (auto callback : callbacks) {
		if (callback != nullptr) {
			callback(arg);
		}
	}
}


std::vector<Collider> LoadCollidersFromConfig() {
	std::ifstream config(Game::GetAbsolutePath("config/collision.json"));
	std::stringstream configBuffer;
	configBuffer << config.rdbuf();

	try {
		nlohmann::json json;
		json = nlohmann::json::parse(configBuffer.str());
		auto j = json["colliders"];
		std::vector<Collider> returnValue;
		// iterate the array
		for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
			//std::cout << *it << '\n';
			nlohmann::json value = it.value();
			Collider c;
			float collisionSizeX = value["width"].get<float>();
			float collisionSizeY = value["height"].get<float>();
			c.drawable.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
			float posX = value["x"].get<float>();
			float posY = value["y"].get<float>();
			c.transform.SetWorldPosition(posX, posY);
			c.colliderType = (ColliderType)value["type"].get<int>();
			c.collisionDirection = (CollisionDirection)value["direction"].get<int>();

			if(value.contains("texture"))
			{
				std::string texturePath = value["texture"].get<std::string>();
				if(!texturePath.empty())
				{
					auto texture = Assets::LoadTexture(texturePath);
					// texture.setRepeated(true);
					c.drawable.setTexture(&Assets::LoadTexture(texturePath));
				}
			}

			if (value.contains("draw"))
			{
				c.draw = value["draw"].get<bool>();
				if(c.draw && c.drawable.getTexture() == nullptr)
				{
					c.drawable.setFillColor(sf::Color(155, 103, 60));
					c.drawable.setOutlineThickness(1.f);
					c.drawable.setOutlineColor(sf::Color(50, 50, 50, 255));
				}
			}
			
			returnValue.push_back(c);
		}
		return returnValue;

	}
	catch (const std::exception& e) {
		std::cout << "Error opening config file." << std::endl;
		std::cerr << e.what() << std::endl;
		return std::vector<Collider>();
	}
}

