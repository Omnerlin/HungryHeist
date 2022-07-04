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
	GameGuiState GetGuiState();
	void BuildMenus();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void SetScore(int score);
	sf::Font* gameFont;

	GuiButton PlayButton;
	GuiButton QuitButton;
	GuiButton ReplayButton;
	GuiButton EndQuitButton;
	GuiButton HomeButton;
	GUIText ScoreText;

private:
	GameGuiState _guiState = GameGuiState::None;
	GuiElement home;
	GuiElement end;
};
