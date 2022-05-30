#pragma once
#include "Gui/GuiTransform.h"

struct GuiElement : public sf::Drawable, public GuiTransform {
	virtual void UpdateElementBasedOnTransform() {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void HandleMouseDown();
	virtual void HandleMouseUp();
	virtual void HandleMouseEnter();
	virtual void HandleMouseExit();

	bool _captureEvents{ false };
	bool _isActive{ true };

	sf::Color mouseDownColor = sf::Color::Red;
	sf::Color hoverColor = sf::Color::Black;
	sf::Color defaultColor = sf::Color::Cyan;
};