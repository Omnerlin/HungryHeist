#pragma once
#include "GuiElement.h"
#include "GuiText.h"
#include <functional>
#include <SFML/Audio/Sound.hpp>

struct GuiButton : public GuiElement
{
	virtual void HandleMouseDown() override;
	virtual void HandleMouseUp() override;
	virtual void HandleMouseEnter() override;
	virtual void HandleMouseExit() override;
	virtual void InitElement() override;

	sf::Sound hoverSound;
	sf::Sound mouseDownSound;
	sf::Sound mouseUpSound;

	std::vector<std::function<void()>> onClick;
	GuiElement visualElement;
	GUIText text;
	sf::Texture* buttonTexture;
};
