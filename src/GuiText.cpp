#include "Gui/GuiText.h"

sf::Vector2f GUIText::FindCharacterPositionLocal(size_t index)
{
	return GetWorldTransform().getInverseTransform().transformPoint(text.findCharacterPos(index));
}



void GUIText::UpdateElementBasedOnTransform()
{
	// Test and see if our text needs to wrap.
	for (int i = 0; i < text.getString().getSize(); i++) {
		sf::Vector2f characterLocalPos = FindCharacterPositionLocal(i);
	}
}

void GUIText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GuiElement::draw(target, states);
	target.draw(text, states);
}

void GUIText::UpdateTransforms()
{
	GuiElement::UpdateTransforms();
	text.setOrigin(GetOrigin());
	text.setPosition(GetWorldPosition());
	text.setRotation(GetWorldRotation());
	text.setScale(GetWorldScale());
}

void GUIText::CopyTextAttributesFrom(GUIText& other)
{
	CopyAttributesFrom(other);
	text.setFont(*other.text.getFont());
	text.setFillColor(other.text.getFillColor());
	text.setOutlineColor(other.text.getOutlineColor());
	text.setOutlineThickness(other.text.getOutlineThickness());
	text.setCharacterSize(other.text.getCharacterSize());
}
