#pragma once
#include <EntityTransform.h>
#include <Collider.h>

enum FoodType {
	Apple,
	Beans,
	Pizza,
	IceCream,
	Bread,
	FOODTYPES_COUNT
};

struct FoodItem : sf::Drawable {
	EntityTransform transform;
	DrawnEntity<sf::Sprite> foodSprite;
	DrawnEntity<sf::Sprite> glowSprite;
	Collider collider;
	void Initialize();
	void AssignRandomType();
	void AssignType(FoodType type);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Inherited via Drawable

private:
	int randomIndex{0};
};
