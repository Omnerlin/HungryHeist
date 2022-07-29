#include "Gui/GuiElement.h"
#include <iostream>

void GuiElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GetRectDrawable(), states);
}

void GuiElement::UpdateTransforms()
{
	GuiTransform::UpdateTransforms();
	UpdateElementBasedOnTransform();
}

void GuiElement::HandleMouseDown()
{
	SetColor(mouseDownColor);
}

void GuiElement::HandleMouseUp()
{
	SetColor(hoverColor);
}

void GuiElement::HandleMouseEnter()
{
	SetColor(hoverColor);
}

void GuiElement::HandleMouseExit()
{
	SetColor(defaultColor);
}

void GuiElement::HandleMouseDrag(float deltaX, float deltaY)
{
}

void GuiElement::SetActiveHierarchy(bool active, bool shouldCaptureEvents)
{
	isActive = active;
	captureEvents = shouldCaptureEvents;
	if(!_children.empty())
	{
		for(auto child : _children)
		{
			dynamic_cast<GuiElement*>(child)->SetActiveHierarchy(active, shouldCaptureEvents);
		}
	}
}

void GuiElement::SetChildrenActive(bool active, bool shouldCaptureEvents)
{
	isActive = active;
	if (!_children.empty())
	{
		for (auto child : _children)
		{
			dynamic_cast<GuiElement*>(child)->SetActiveHierarchy(active, shouldCaptureEvents);
		}
	}
}
