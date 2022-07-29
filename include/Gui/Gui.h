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

	void ProcessEvent(sf::Event& event);
	//GuiElement* UpdateHoveredElement();
	void UpdateHoveredElementNew();
	GuiElement* UpdateHoveredElement(GuiElement* element);
	const sf::Vector2f& GetMousePosition();

private:
	void DrawElement(GuiElement* element, sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::Vector2f _mousePosition;
	sf::Vector2f _mouseDelta;
};