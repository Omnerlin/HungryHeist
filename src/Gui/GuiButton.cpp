#include "Gui/GuiButton.h"

void GuiButton::HandleMouseDown()
{
	visualElement.SetLocalScale({ 0.9f, 0.9f });
}

void GuiButton::HandleMouseUp()
{
	visualElement.SetLocalScale({ 1.1f, 1.1f });
	for (auto c : onClick) {
		if (c != nullptr) {
			c();
		}
	}
}

void GuiButton::HandleMouseEnter()
{
	visualElement.SetLocalScale({ 1.1f, 1.1f });
}

void GuiButton::HandleMouseExit()
{
	visualElement.SetLocalScale({ 1,1 });
}

void GuiButton::InitElement()
{
	visualElement.SetRectSize(GetRectSize(), false);
	visualElement.SetParent(this);
	visualElement.SetAnchorMin(0, 0);
	visualElement.SetAnchorMax(1, 1);
	visualElement.SetPivot({ 0.5f, 0.5f });
	visualElement.SetLocalPosition(visualElement.GetRectSize().x / 2, visualElement.GetRectSize().y / 2);
	visualElement.captureEvents = false;
	visualElement.SetColor(sf::Color::White);

	//text.SetColor(sf::Color::Magenta);
	text.SetParent(&visualElement);
	text.text.setString("PLAY");
	text.SetPivot(visualElement.GetPivot());
	text.SetRectSize(100, 50);
	text.SetLocalPosition(visualElement.GetLocalPosition().x - 30, visualElement.GetLocalPosition().y - 18);
	text.SetAnchorMin(0.5, 0.5);
	text.SetAnchorMax(0.5, 0.5);
	text.text.setFillColor(sf::Color::Black);

}
