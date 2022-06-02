#include "Gui/GameGui.h"

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
		ScoreText.SetActiveHierarchy(false, false);
	}

	switch (state)
	{
	case None:
		break;
	case GameGuiState::Home:
		home.SetChildrenActive(true, true);
		PlayButton.visualElement.captureEvents = false;
		QuitButton.visualElement.captureEvents = false;
		break;
	case GameGuiState::End:
		end.SetChildrenActive(true, true);
		break;
	case GameGuiState::Play:
		ScoreText.SetActiveHierarchy(true, true);
	}

	_guiState = state;
}

void GameGui::BuildMenus()
{
	// ===== Build End Menu =====
	end.SetParent(root);
	end.CopyAttributesFrom(home);


	ReplayButton.SetParent(&end);
	ReplayButton.SetRectSize(300, 50);
	ReplayButton.SetPivot({ 0.5f, 0.5f });
	ReplayButton.SetAnchorMin(0.4f, 0.5f);
	ReplayButton.SetAnchorMax(0.6f, 0.5f);
	ReplayButton.SetLocalPosition(1280.f / 2, 720.f * 0.45f);
	ReplayButton.text.text.setString("REPLAY");
	ReplayButton.InitElement();
	ReplayButton.text.text.setFont(gameFont);


	EndQuitButton.SetParent(&end);
	EndQuitButton.CopyAttributesFrom(PlayButton);
	EndQuitButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	EndQuitButton.text.text.setFont(gameFont);
	EndQuitButton.InitElement();
	EndQuitButton.text.text.setString("QUIT");

	ScoreText.SetParent(root);
	ScoreText.text.setFillColor(sf::Color::White);
	ScoreText.text.setFont(gameFont);
	ScoreText.text.setString("Food Eaten: 0");
	ScoreText.text.setCharacterSize(60);
	ScoreText.SetLocalPosition(50, 25);


	// ===== Build Home Menu =====
	home.CopyAttributesFrom(*root);
	home.SetParent(root);
	home.SetAnchorMin(0, 0);
	home.SetAnchorMax(1, 1);


	PlayButton.SetParent(&home);
	PlayButton.SetRectSize(300, 50);
	PlayButton.SetPivot({ 0.5f, 0.5f });
	PlayButton.SetAnchorMin(0.4f, 0.5f);
	PlayButton.SetAnchorMax(0.6f, 0.5f);
	PlayButton.SetLocalPosition(1280.f / 2, 720.f * 0.45f);
	PlayButton.InitElement();
	PlayButton.text.text.setFont(gameFont);
	PlayButton.SetColor(sf::Color::Green);
	PlayButton.captureEvents = true;


	QuitButton.SetParent(&home);
	QuitButton.CopyAttributesFrom(PlayButton);
	QuitButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	QuitButton.text.text.setFont(gameFont);
	QuitButton.InitElement();
	QuitButton.text.text.setString("QUIT");



	home.SetChildrenActive(true, true);
	end.SetActiveHierarchy(false, false);
	ScoreText.SetActiveHierarchy(false, false);
}

void GameGui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Gui::draw(target, states);

}
