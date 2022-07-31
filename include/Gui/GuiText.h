#pragma once
#include "Gui/GuiElement.h"
#include <string>

struct GUIText : public GuiElement
{
	sf::Text text;
	sf::Vector2f FindCharacterPositionLocal(size_t index);
	std::string desiredText;
	virtual void UpdateElementBasedOnTransform() override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void UpdateTransforms() override;
	void CopyTextAttributesFrom(GUIText& other);
};

