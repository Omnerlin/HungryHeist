#pragma once
#include "GuiElement.h"
#include "GuiText.h"
#include <functional>
#include "GameSound.h"

struct GuiButton : public GuiElement
{
	virtual void HandleMouseDown() override;
	virtual void HandleMouseUp() override;
	virtual void HandleMouseEnter() override;
	virtual void HandleMouseExit() override;
	virtual void InitElement() override;
	void CopyButtonAttributesFrom(GuiButton& other);

	GameSound hoverSound;
	GameSound mouseDownSound;
	GameSound mouseUpSound;

	std::vector<std::function<void()>> onClick;
	GuiElement visualElement;
	GUIText text;
	sf::Texture* buttonBaseTexture;
	sf::Texture* buttonHoverTexture;
};
