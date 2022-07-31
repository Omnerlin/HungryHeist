#include "Gui/GameGui.h"

#include "Game.h"

void GameGui::SetGuiState(GameGuiState state)
{
	if (state == _guiState) return;

	switch (_guiState)
	{
	case None:
		break;
	case GameGuiState::Home:
		home.SetChildrenActive(false, false);
		break;
	case GameGuiState::End:
		end.SetChildrenActive(false, false);
		break;
	case GameGuiState::Play:
		scoreText.SetActiveHierarchy(false, false);
		break;
	case GameGuiState::Pause:
		pausedPanel.SetActiveHierarchy(false, false);
		end.SetChildrenActive(false, false);
		break;
	case GameGuiState::Settings:
		settingsPanel.SetActiveHierarchy(false, false);
		break;
	}

	switch (state)
	{
	case None:
		break;
	case GameGuiState::Home:
		home.SetChildrenActive(true, true);
		playButton.visualElement.captureEvents = false;
		quitButton.visualElement.captureEvents = false;
		fullscreenToggle.visualElement.captureEvents = false;
		settingsButton.visualElement.captureEvents = false;
		break;
	case GameGuiState::End:
		end.SetChildrenActive(true, true);
		replayButton.visualElement.captureEvents = false;
		endQuitButton.visualElement.captureEvents = false;
		homeButton.visualElement.captureEvents = false;
		gameOverStats.SetLocalPosition(end.GetRectSize().x / 2.f - gameOverStats.text.getGlobalBounds().width / 2.f, gameOverStats.GetLocalPosition().y);
		break;
	case GameGuiState::Play:
		scoreText.SetActiveHierarchy(true, true);
		break;
	case GameGuiState::Pause:
		pausedPanel.SetChildrenActive(true, true);
		end.SetChildrenActive(true, true);
		gameOverStats.isActive = false;
		gameOverText.isActive = false;
		replayButton.visualElement.captureEvents = false;
		endQuitButton.visualElement.captureEvents = false;
		homeButton.visualElement.captureEvents = false;
		break;
	case GameGuiState::Settings:
		settingsPanel.SetChildrenActive(true, true);
		fullscreenToggle.visualElement.captureEvents = false;
		settingsReturnButton.visualElement.captureEvents = false;
		break;
	}

	_guiState = state;
}

GameGuiState GameGui::GetGuiState()
{
	return _guiState;
}

void GameGui::BuildMenus()
{
	// ==== Build Pause Menu ====
	pausedPanel.SetParent(root);
	pausedPanel.CopyAttributesFrom(*root);
	pausedPanel.SetColor(sf::Color(0, 0, 0, 125));
	pausedPanel.SetAnchorMin(0, 0);
	pausedPanel.SetAnchorMax(1, 1);

	pausedText.SetParent(&pausedPanel);
	pausedText.text.setFillColor(sf::Color::White);
	pausedText.text.setFont(*gameFont);
	pausedText.text.setString("PAUSED");
	pausedText.text.setOutlineThickness(3.f);
	pausedText.text.setCharacterSize(120);
	pausedText.SetLocalPosition(pausedPanel.GetRectSize().x / 2.f - pausedText.text.getGlobalBounds().width / 2.f, 50);
	pausedText.SetAnchorMin(0.5f, 0.25f);
	pausedText.SetAnchorMax(0.5f, 0.25f);

	// ===== Build Home Menu =====
	home.CopyAttributesFrom(*root);
	home.SetParent(root);
	home.SetAnchorMin(0, 0);
	home.SetAnchorMax(1, 1);

	titleParent.SetParent(&home);
	titleParent.SetColor(sf::Color::Transparent);
	titleParent.SetLocalPosition(pausedPanel.GetRectSize().x / 2.f, 50);
	titleParent.SetAnchorMin(0.5f, 0.25f);
	titleParent.SetAnchorMax(0.5f, 0.25f);

	title.SetParent(&titleParent);
	title.text.setFillColor(sf::Color::White);
	title.text.setFont(*gameFont);
	title.text.setString("HUNGRY HEIST");
	title.text.setOutlineThickness(3.f);
	title.text.setCharacterSize(120);
	title.SetLocalPosition(-title.text.getGlobalBounds().width / 2.f, 0);
	title.SetAnchorMin(0.5f, 0.25f);
	title.SetAnchorMax(0.5f, 0.25f);

	playButton.SetParent(&home);
	playButton.SetRectSize(300, 50);
	playButton.SetPivot({ 0.5f, 0.5f });
	playButton.SetAnchorMin(0.4f, 0.5f);
	playButton.SetAnchorMax(0.6f, 0.5f);
	playButton.SetLocalPosition(1280.f / 2, 720.f * 0.45f);
	playButton.text.text.setFont(*gameFont);
	playButton.text.text.setString("PLAY");
	playButton.captureEvents = true;
	playButton.InitElement();

	settingsButton.SetParent(&home);
	settingsButton.SetRectSize(300, 50);
	settingsButton.SetPivot({ 0.5f, 0.5f });
	settingsButton.SetAnchorMin(0.4f, 0.5f);
	settingsButton.SetAnchorMax(0.6f, 0.5f);
	settingsButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	settingsButton.text.text.setFont(*gameFont);
	settingsButton.text.text.setString("SETTINGS");
	settingsButton.captureEvents = true;
	settingsButton.InitElement();

	quitButton.SetParent(&home);
	quitButton.CopyAttributesFrom(playButton);
	quitButton.SetLocalPosition(1280.f / 2, 720.f * 0.65f);
	quitButton.text.text.setFont(*gameFont);
	quitButton.text.text.setString("QUIT");
	quitButton.InitElement();

	// ==== Build Settings Menu ====
	settingsPanel.SetParent(root);
	settingsPanel.CopyAttributesFrom(home);

	settingsTitle.SetParent(&settingsPanel);
	settingsTitle.CopyAttributesFrom(title);
	settingsTitle.text.setFont(*gameFont);
	settingsTitle.text.setString("SETTINGS");
	settingsTitle.text.setOutlineThickness(3.f);
	settingsTitle.text.setCharacterSize(120);
	settingsTitle.SetLocalPosition(pausedPanel.GetRectSize().x / 2.f - settingsTitle.text.getGlobalBounds().width / 2.f, 50);
	settingsTitle.SetAnchorMin(0.5f, 0.25f);
	settingsTitle.SetAnchorMax(0.5f, 0.25f);

	fullscreenToggle.SetParent(&settingsPanel);
	fullscreenToggle.SetPivot({ 0.5f, 0.5f });
	fullscreenToggle.SetRectSize(100, 100);
	fullscreenToggle.SetLocalPosition(1280.f / 2, 720.f * 0.35f);
	fullscreenToggle.SetAnchorMin(0.5f, 0.5f);
	fullscreenToggle.SetAnchorMax(0.5f, 0.5f);
	fullscreenToggle.text.text.setFont(*gameFont);
	fullscreenToggle.text.text.setString("FULLSCREEN");
	fullscreenToggle.InitElement();
	fullscreenToggle.text.text.setCharacterSize(40);
	fullscreenToggle.text.SetLocalPosition(-fullscreenToggle.text.text.getGlobalBounds().width - 50, fullscreenToggle.text.text.getGlobalBounds().height / 2.f);
	fullscreenToggle.SetToggleValue(Game::Instance->settings.fullscreen, false);

	volumeSlider.SetParent(&settingsPanel);
	volumeSlider.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	volumeSlider.SetAnchorMin(0.5f, 0.5f);
	volumeSlider.SetAnchorMax(0.5f, 0.5f);
	volumeSlider.InitElement();
	volumeSlider.SetRectSize(200, volumeSlider.GetRectSize().y);

	volumeText.SetParent(&volumeSlider);
	volumeText.text.setFont(*gameFont);
	volumeText.text.setString("VOLUME");
	volumeText.text.setCharacterSize(40);
	volumeText.SetAnchorMin(0.0f, 0.5f);
	volumeText.SetAnchorMax(0.0f, 0.5f);
	volumeText.SetLocalPosition(-volumeText.text.getGlobalBounds().width - 50, -5);
	volumeText.text.setOutlineThickness(fullscreenToggle.text.text.getOutlineThickness());
	volumeText.text.setOutlineColor(fullscreenToggle.text.text.getOutlineColor());


	settingsReturnButton.SetParent(&settingsPanel);
	settingsReturnButton.SetRectSize(300, 50);
	settingsReturnButton.SetPivot({ 0.5f, 0.5f });
	settingsReturnButton.SetAnchorMin(0.4f, 0.5f);
	settingsReturnButton.SetAnchorMax(0.6f, 0.5f);
	settingsReturnButton.SetLocalPosition(1280.f / 2, 720.f * 0.75f);
	settingsReturnButton.text.text.setFont(*gameFont);
	settingsReturnButton.text.text.setString("RETURN");
	settingsReturnButton.captureEvents = true;
	settingsReturnButton.InitElement();

	// ===== Build End Menu =====
	end.SetParent(root);
	end.CopyAttributesFrom(home);

	gameOverText.SetParent(&end);
	gameOverText.text.setFillColor(sf::Color::White);
	gameOverText.text.setFont(*gameFont);
	gameOverText.text.setString("GAME OVER");
	gameOverText.text.setOutlineThickness(3.f);
	gameOverText.text.setCharacterSize(80);
	gameOverText.SetLocalPosition(pausedPanel.GetRectSize().x / 2.f - gameOverText.text.getGlobalBounds().width / 2.f, 30);
	gameOverText.SetAnchorMin(0.5f, 0.25f);
	gameOverText.SetAnchorMax(0.5f, 0.25f);

	gameOverStats.SetParent(&end);
	gameOverStats.text.setFillColor(sf::Color::White);
	gameOverStats.text.setFont(*gameFont);
	gameOverStats.text.setString("Food Eaten:");
	gameOverStats.text.setOutlineThickness(3.f);
	gameOverStats.text.setCharacterSize(40);
	gameOverStats.SetLocalPosition(pausedPanel.GetRectSize().x / 2.f - gameOverStats.text.getGlobalBounds().width / 2.f, 150);
	gameOverStats.SetAnchorMin(0.5f, 0.25f);
	gameOverStats.SetAnchorMax(0.5f, 0.25f);

	replayButton.SetParent(&end);
	replayButton.SetRectSize(300, 50);
	replayButton.SetPivot({ 0.5f, 0.5f });
	replayButton.SetAnchorMin(0.4f, 0.5f);
	replayButton.SetAnchorMax(0.6f, 0.5f);
	replayButton.SetLocalPosition(1280.f / 2, 720.f * 0.45f);
	replayButton.text.text.setFont(*gameFont);
	replayButton.text.text.setString("REPLAY");
	replayButton.InitElement();

	homeButton.SetParent(&end);
	homeButton.CopyAttributesFrom(replayButton);
	homeButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	homeButton.text.text.setFont(*gameFont);
	homeButton.text.text.setString("HOME");
	homeButton.InitElement();

	endQuitButton.SetParent(&end);
	endQuitButton.CopyAttributesFrom(playButton);
	endQuitButton.SetLocalPosition(1280.f / 2, 720.f * 0.65f);
	endQuitButton.text.text.setFont(*gameFont);
	endQuitButton.text.text.setString("QUIT");
	endQuitButton.InitElement();

	scoreText.SetParent(root);
	scoreText.text.setFillColor(sf::Color::White);
	scoreText.text.setFont(*gameFont);
	scoreText.text.setString("Food Eaten: 0");
	scoreText.text.setCharacterSize(60);
	scoreText.SetLocalPosition(50, 25);
	scoreText.text.setOutlineThickness(2.f);

	home.SetChildrenActive(true, true);
	end.SetActiveHierarchy(false, false);
	scoreText.SetActiveHierarchy(false, false);
	settingsPanel.SetActiveHierarchy(false, false);
	pausedPanel.SetActiveHierarchy(false, false);
}

void GameGui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Gui::draw(target, states);

}

void GameGui::SetScore(int score)
{
	std::string scoreString = "Food Eaten: " + std::to_string(score);
	scoreText.text.setString(scoreString);
	gameOverStats.text.setString(scoreString);
}
