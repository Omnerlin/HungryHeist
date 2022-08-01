#include "Gui/GuiSlider.h"

#include "Assets.h"
#include "Game.h"

void GuiSlider::HandleMouseDown()
{
	SetValue(GetNormalizedValueFromMouse(), true);
}

void GuiSlider::HandleMouseUp()
{
}

void GuiSlider::HandleMouseDrag(float deltaX, float deltaY)
{
	SetValue(GetNormalizedValueFromMouse(), true);
}

void GuiSlider::HandleMouseDragEnd()
{
	std::cout << "Done dragging" << std::endl;
	if(Game::Instance->gui._hoveredElement != this)
	{
		sliderHandle.SetTexture(baseHandleTexture, true);
		sliderHandle.SetWorldScale(1, 1);
	}
}

void GuiSlider::HandleMouseEnter()
{
	sliderHandle.SetTexture(highlightTexture, true);
	sliderHandle.SetWorldScale(1.25f, 1.25f);
}

void GuiSlider::HandleMouseExit()
{
	if(!Game::Instance->gui.isDragging)
	{
		sliderHandle.SetTexture(baseHandleTexture, true);
		sliderHandle.SetWorldScale(1.0f, 1.0f);
	}
}

void GuiSlider::InitElement()
{
	SetColor(sf::Color::White);
	SetRectSize({ 150,40 });
	SetTexture(Assets::LoadTexture("assets/textures/sliderBG.png"));
	baseHandleTexture = Assets::LoadTexture("assets/textures/button2.png");
	highlightTexture = Assets::LoadTexture("assets/textures/button.png");


	sliderHandle.SetRectSize(20, 30);
	sliderHandle.SetPivot({ 0.5f, 0.5f });
	sliderHandle.SetParent(this);
	sliderHandle.SetLocalPosition(0, GetRectSize().y / 2.f);
	sliderHandle.SetAnchorMin(0.5f, 0.5f);
	sliderHandle.SetAnchorMax(0.5f, 0.5f);
	sliderHandle.SetColor(sf::Color::White);
	sliderHandle.SetTexture(baseHandleTexture, true);
}

void GuiSlider::SetValue(float value, bool notify)
{
	if(currentValue != value)
	{
		currentValue = value;
		sliderHandle.SetLocalPosition(currentValue * GetRectSize().x, sliderHandle.GetLocalPosition().y);
		if (sliderValueChanged != nullptr && notify)
		{
			sliderValueChanged(value);
		}
	}
}

float GuiSlider::GetNormalizedValueFromMouse()
{
	auto mousePosDiff = Game::Instance->gui.GetMousePosition().x - GetWorldPosition().x;
	return std::clamp(mousePosDiff / GetRectSize().x, 0.f, 1.f);
}
