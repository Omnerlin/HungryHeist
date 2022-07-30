#include "FoodItem.h"
#include "Assets.h"
#include "Physics.h"

void FoodItem::Initialize()
{
	transform.SetOrigin(8, 8);
	collider.drawable.setSize(sf::Vector2f(16, 16));
	collider.colliderType = ColliderType::Trigger;
	collider.transform.SetOrigin(8, 8);
	collider.transform.SetParent(&transform);
	foodSprite.transform.SetParent(&transform);
	foodSprite.transform.SetOrigin(8, 8);
	glowSprite.transform.SetParent(&foodSprite.transform);
	glowSprite.transform.SetLocalScale(0.25f, 0.25f);
	glowSprite.transform.SetOrigin(256.f / 2, 256.f / 2);
	foodSprite.drawable.setTexture(Assets::LoadTexture("assets/textures/food.png"));
	glowSprite.drawable.setTexture(Assets::LoadTexture("assets/textures/flare.png"));
	collider.drawable.setFillColor(sf::Color(255, 255, 255, 100));
	AssignType(FoodType::Pizza);
	Physics::RegisterCollider(&collider);
}

void FoodItem::AssignRandomType()
{
	randomIndex = (randomIndex + 1) % FoodType::FOODTYPES_COUNT;
	AssignType(static_cast<FoodType>(randomIndex));
}

void FoodItem::AssignType(FoodType type)
{
	randomIndex = type;
	foodSprite.drawable.setTextureRect(sf::IntRect(16 * type, 0, 16, 16));
}

void FoodItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(glowSprite, states);
	target.draw(foodSprite, states);
}


