#include "FoodItem.h"

sf::Texture FoodItem::FoodTexture = sf::Texture();
sf::Texture FoodItem::GlowTexture = sf::Texture();

void FoodItem::AssignRandomType()
{
	randomIndex = (randomIndex + 1) % FoodType::FOODTYPES_COUNT;
	AssignType((FoodType)randomIndex);
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


