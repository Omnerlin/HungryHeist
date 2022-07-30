#include "Gui/GuiToggle.h"
#include "Assets.h"
#include "Game.h"

void GuiToggle::InitElement()
{
	GuiButton::InitElement();
	onClick.emplace_back([this]() { SetToggleValue(!isToggled, true); });
	UpdateButtonTextures();
}

void GuiToggle::SetToggleValue(bool toggleValue, bool notify)
{
	if (toggleValue == isToggled) return;
	isToggled = toggleValue;
	UpdateButtonTextures();
	if(notify)
	{
		if(onToggleChanged != nullptr)
		{
			onToggleChanged(toggleValue);
		}
	}
}

bool GuiToggle::IsToggled() const
{
	return isToggled;
}

void GuiToggle::UpdateButtonTextures()
{
	if (isToggled)
	{
		buttonBaseTexture = &Assets::LoadTexture("assets/textures/checkBlue.png");
		buttonHoverTexture = &Assets::LoadTexture("assets/textures/checkGreen.png");
	}
	else
	{
		buttonBaseTexture = &Assets::LoadTexture("assets/textures/xBlue.png");
		buttonHoverTexture = &Assets::LoadTexture("assets/textures/xGreen.png");
	}

	if(Game::Instance->gui._hoveredElement == this)
	{
		visualElement.SetTexture(*buttonHoverTexture, true);
	}
	else
	{
		visualElement.SetTexture(*buttonBaseTexture, true);
	}
}
