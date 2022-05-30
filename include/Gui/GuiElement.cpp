#include "Gui/GuiElement.h"

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GetRectDrawable(), states);
}

void GuiElement::HandleMouseDown()
{
	SetDebugColor(mouseDownColor);
}

void GuiElement::HandleMouseUp()
{
	SetDebugColor(hoverColor);
}

void GuiElement::HandleMouseEnter()
{
	SetDebugColor(hoverColor);
}

void GuiElement::HandleMouseExit()
{
	SetDebugColor(defaultColor);
}
