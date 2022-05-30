#pragma once
#include <SFML/Graphics.hpp>
#include "Gui/GuiTransform.h"
#include <Gui/GuiElement.h>

// This class will handle GUI events as well as draw (and update) all of its GUI elements
struct Gui : sf::Drawable {

public:
	GuiElement* root;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Inherited from Drawable

	GuiElement* _hoveredElement{ nullptr };
	GuiElement* _clickedElement{ nullptr };
	GuiElement* _releasedElement{ nullptr };

	void ProcessEvent(sf::Event& event);
	void UpdateHoveredElement();

private:
	void DrawElement(GuiElement* element, sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Vector2f _mousePosition;
};