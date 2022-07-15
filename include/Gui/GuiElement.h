#pragma once
#include "Gui/GuiTransform.h"

struct GuiElement : public sf::Drawable, public GuiTransform {
	virtual void UpdateElementBasedOnTransform() {};
	virtual void InitElement() {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void UpdateTransforms() override;
	virtual void HandleMouseDown();
	virtual void HandleMouseUp();
	virtual void HandleMouseEnter();
	virtual void HandleMouseExit();
	virtual void HandleMouseDrag(float deltaX, float deltaY);
	void SetActiveHierarchy(bool active, bool captureEvents);

	void SetChildrenActive(bool active, bool shouldCaptureEvents);

	bool captureEvents{ false };
	bool isActive{ true };

	sf::Color mouseDownColor = sf::Color::Red;
	sf::Color hoverColor = sf::Color::Black;
	sf::Color defaultColor = sf::Color::Cyan;


};