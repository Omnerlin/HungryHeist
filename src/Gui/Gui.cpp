#include "Gui/Gui.h"
#include <iostream>

void Gui::ProcessEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		_mousePosition = { (float)event.mouseMove.x, (float)event.mouseMove.y };
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			// Click on our hovered element if we have it
			if (_hoveredElement != nullptr) {
				_clickedElement = _hoveredElement;
				_hoveredElement->HandleMouseDown();
			}
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			// Click on our hovered element if we have it
			if (_hoveredElement != nullptr) {
				if (_hoveredElement == _clickedElement) {
					_hoveredElement->HandleMouseUp();
				}
			}
			_clickedElement = nullptr;
		}
	}
}

void Gui::UpdateHoveredElementNew()
{
	GuiElement* element = UpdateHoveredElement(root);

	if (element != _hoveredElement)
	{
		if (_hoveredElement != nullptr) {
			_hoveredElement->HandleMouseExit();
		}
		if (element != nullptr)
		{
			element->HandleMouseEnter();
		}
		_hoveredElement = element;
	}
}

GuiElement* Gui::UpdateHoveredElement(GuiElement* element)
{
	GuiElement* result = nullptr;

	if (!element->isActive) return nullptr;
	auto children = element->GetChildren();
	for (auto childItr = children->rbegin(); childItr != children->rend(); childItr++) {
		GuiElement* test = dynamic_cast<GuiElement*>(*childItr);
		if (test == nullptr) continue;
		if (!test->GetChildren()->empty())
		{
			result = UpdateHoveredElement(test);
			if (result != nullptr)
			{
				return result;
			}
		}
	}

	if (result != nullptr)
	{
		return result;
	}
	if (element->isActive && element->captureEvents && element->GetRectDrawable().getGlobalBounds().contains(_mousePosition)) {
		return element;
	}

	return nullptr;
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	DrawElement(root, target, states);
}

void Gui::DrawElement(GuiElement* element, sf::RenderTarget& target, sf::RenderStates states) const
{
	if ((*element).isActive) {
		target.draw(*element, states);
		auto children = element->GetChildren();
		if (!children->empty()) {
			for (auto itr = children->begin(); itr != children->end(); itr++) {
				DrawElement(static_cast<GuiElement*>(*itr), target, states);
			}
		}
	}
}
