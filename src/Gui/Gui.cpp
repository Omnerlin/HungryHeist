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
				_hoveredElement->HandleMouseDown();
			}
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			// Click on our hovered element if we have it
			if (_hoveredElement != nullptr) {
				_hoveredElement->HandleMouseUp();
			}
		}
	}
}

void Gui::UpdateHoveredElement()
{
	EntityTransform* current = root->GetDeepestChild();
	while (current->GetParent() != nullptr) {

		auto children = current->GetParent()->GetChildren();
		for (auto childItr = children->rbegin(); childItr != children->rend(); childItr++) {
			GuiElement* test = dynamic_cast<GuiElement*>(*childItr);
			if (current == nullptr) continue;
			if (test->GetRectDrawable().getGlobalBounds().contains(_mousePosition)) {
				if (_hoveredElement != test) {
					if (_hoveredElement != nullptr) {
						_hoveredElement->HandleMouseExit();
						//_hoveredElement->SetDebugColor(sf::Color::Green);
					}
					test->HandleMouseEnter(); //SetDebugColor(sf::Color::Magenta);
					_hoveredElement = test;
				}
				return;
			}
		}
		current = current->GetParent();
	}
	if (_hoveredElement != nullptr) {
		_hoveredElement->HandleMouseExit();
	}
	_hoveredElement = nullptr;
	
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (root == nullptr) return;
	DrawElement(root, target, states);
}

void Gui::DrawElement(GuiElement* element, sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*element, states);
	auto children = element->GetChildren();
	if (!children->empty()) {
		for (auto itr = children->begin(); itr != children->end(); itr++) {
			DrawElement(static_cast<GuiElement*>(*itr), target, states);
		}
	}
}
