#pragma once
#include "GuiButton.h"

struct GuiToggle : GuiButton
{
	virtual void InitElement() override;
	void SetToggleValue(bool toggleValue, bool notify);
	bool IsToggled() const;
	std::function<void(bool)> onToggleChanged;

private:
	bool isToggled{false};
	void UpdateButtonTextures();
};
