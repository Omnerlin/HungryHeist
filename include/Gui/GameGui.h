#pragma once
#include "Gui.h"
#include "Gui/GuiButton.h"
#include "Gui/GuiSlider.h"
#include "Gui/GuiToggle.h"

enum GameGuiState
{
	None,
	Home,
	Play,
	End,
	Pause,
	Settings,
	Credits
};


struct GameGui : Gui
{
	void SetGuiState(GameGuiState state);
	GameGuiState GetGuiState();
	void BuildMenus();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void SetScore(int score);
	sf::Font* gameFont;

	GuiElement titleParent;
	GUIText title;
	GuiButton playButton;
	GuiButton quitButton;
	GuiButton replayButton;
	GuiButton endQuitButton;
	GuiButton homeButton;
	GuiButton creditsButton;
	GUIText scoreText;

	GUIText pausedText;
	GuiElement pausedPanel;

	GuiElement settingsPanel;
	GUIText settingsTitle;
	GuiButton settingsButton;
	GuiSlider volumeSlider;
	GuiToggle fullscreenToggle;
	GuiButton settingsReturnButton;
	GUIText volumeText;
	
	GUIText gameOverText;
	GUIText gameOverStats;

	GuiElement creditsPanel;
	GUIText creditsTitle;
	std::vector<GUIText> guiTexts;
	GuiButton creditsReturnButton;

private:
	GameGuiState _guiState = GameGuiState::None;
	GuiElement home;
	GuiElement end;
};
