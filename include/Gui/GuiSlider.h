#pragma once
#include "GuiButton.h"
#include "GuiElement.h"

struct GuiSlider : GuiElement
{
	GuiButton sliderHandle;
	std::function<void(float)> sliderValueChanged; // normalized (0-1)
	float currentValue;
	void HandleMouseDown() override;
	void HandleMouseUp() override;
	void HandleMouseDrag(float deltaX, float deltaY) override;
	void HandleMouseEnter() override;
	void HandleMouseExit() override;
	void InitElement() override;
	void SetValue(float value, bool notify);
	float GetNormalizedValueFromMouse();
};