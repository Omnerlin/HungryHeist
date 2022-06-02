#pragma once
#include "GuiElement.h"
#include "GuiText.h"
#include <functional>

struct GuiButton : public GuiElement
{
	virtual void HandleMouseDown() override;
	virtual void HandleMouseUp() override;
	virtual void HandleMouseEnter() override;
	virtual void HandleMouseExit() override;
	virtual void InitElement() override;
	std::vector<std::function<void()>> onClick;
	GuiElement visualElement;
	GUIText text;
	sf::Texture* buttonTexture;
};