#include "Gui/GuiButton.h"

#include "Assets.h"

void GuiButton::HandleMouseDown()
{
	visualElement.SetLocalScale({ 0.9f, 0.9f });
	mouseDownSound.play();
}

void GuiButton::HandleMouseUp()
{
	visualElement.SetLocalScale({ 1.1f, 1.1f });
	for (auto c : onClick) {
		if (c != nullptr) {
			c();
		}
	}
	mouseUpSound.play();
}

void GuiButton::HandleMouseEnter()
{
	hoverSound.play();
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

	text.SetParent(&visualElement);
	text.SetPivot(visualElement.GetPivot());
	text.SetRectSize(100, 50);
	text.SetAnchorMin(0.5, 0.5);
	text.SetAnchorMax(0.5, 0.5);
	text.SetLocalPosition(visualElement.GetLocalPosition().x - text.text.getLocalBounds().width / 2.f, text.text.getLocalBounds().height / 3.f);
	text.text.setFillColor(sf::Color::Black);

	hoverSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/UI/rollover4.wav"));
	hoverSound.setVolume(600);
	mouseDownSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/Switches/switch24.wav"));
	mouseUpSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/UI/rollover2.wav"));
	mouseUpSound.setVolume(600);
}
