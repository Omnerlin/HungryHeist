#pragma once
#include "Gui.h"
#include "Gui/GuiButton.h"

enum GameGuiState
{
	None,
	Home,
	Play,
	End,
};


struct GameGui : Gui
{
	void SetGuiState(GameGuiState state);
	void BuildMenus();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Font gameFont;

	GuiButton PlayButton;
	GuiButton QuitButton;
	GuiButton ReplayButton;
	GuiButton EndQuitButton;
	GUIText ScoreText;

private:
	GameGuiState _guiState = GameGuiState::None;
	GuiElement home;
	GuiElement end;
};
