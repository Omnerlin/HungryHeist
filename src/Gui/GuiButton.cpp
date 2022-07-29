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
	visualElement.SetTexture(*buttonHoverTexture);
}

void GuiButton::HandleMouseExit()
{
	visualElement.SetLocalScale({ 1,1 });
	visualElement.SetTexture(*buttonBaseTexture);
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
	buttonBaseTexture = &Assets::LoadTexture("assets/textures/button2.png");
	buttonHoverTexture = &Assets::LoadTexture("assets/textures/button.png");

	visualElement.SetTexture(*buttonBaseTexture);

	text.SetParent(&visualElement);
	text.SetPivot(visualElement.GetPivot());
	text.SetRectSize(100, 50);
	text.SetAnchorMin(0.5, 0.5);
	text.SetAnchorMax(0.5, 0.5);
	text.SetLocalPosition(visualElement.GetLocalPosition().x - text.text.getLocalBounds().width / 2.f, text.text.getLocalBounds().height / 4.f);
	text.text.setOutlineColor(sf::Color(75,75,75,255));
	text.text.setOutlineThickness(2);
	text.text.setFillColor(sf::Color::White);

	hoverSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/UI/rollover4.wav"));
	hoverSound.SetBaseVolume(100);
	mouseDownSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/Switches/switch24Fast.wav"));
	mouseUpSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/UI/rollover2.wav"));
	mouseUpSound.SetBaseVolume(100);
}
