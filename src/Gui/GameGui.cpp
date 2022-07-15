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
		ReplayButton.visualElement.captureEvents = false;
		EndQuitButton.visualElement.captureEvents = false;
		HomeButton.visualElement.captureEvents = false;
		break;
	case GameGuiState::Play:
		ScoreText.SetActiveHierarchy(true, true);
	}

	_guiState = state;
}

GameGuiState GameGui::GetGuiState()
{
	return _guiState;
}

void GameGui::BuildMenus()
{
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
	PlayButton.text.text.setFont(*gameFont);
	PlayButton.text.text.setString("PLAY");
	PlayButton.captureEvents = true;
	PlayButton.InitElement();


	QuitButton.SetParent(&home);
	QuitButton.CopyAttributesFrom(PlayButton);
	QuitButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	QuitButton.text.text.setFont(*gameFont);
	QuitButton.text.text.setString("QUIT");
	QuitButton.InitElement();

	// ===== Build End Menu =====
	end.SetParent(root);
	end.CopyAttributesFrom(home);


	ReplayButton.SetParent(&end);
	ReplayButton.SetRectSize(300, 50);
	ReplayButton.SetPivot({ 0.5f, 0.5f });
	ReplayButton.SetAnchorMin(0.4f, 0.5f);
	ReplayButton.SetAnchorMax(0.6f, 0.5f);
	ReplayButton.SetLocalPosition(1280.f / 2, 720.f * 0.45f);
	ReplayButton.text.text.setFont(*gameFont);
	ReplayButton.text.text.setString("REPLAY");
	ReplayButton.InitElement();

	HomeButton.SetParent(&end);
	HomeButton.CopyAttributesFrom(ReplayButton);
	HomeButton.SetLocalPosition(1280.f / 2, 720.f * 0.55f);
	HomeButton.text.text.setFont(*gameFont);
	HomeButton.text.text.setString("HOME");
	HomeButton.InitElement();


	EndQuitButton.SetParent(&end);
	EndQuitButton.CopyAttributesFrom(PlayButton);
	EndQuitButton.SetLocalPosition(1280.f / 2, 720.f * 0.65f);
	EndQuitButton.text.text.setFont(*gameFont);
	EndQuitButton.text.text.setString("QUIT");
	EndQuitButton.InitElement();

	ScoreText.SetParent(root);
	ScoreText.text.setFillColor(sf::Color::White);
	ScoreText.text.setFont(*gameFont);
	ScoreText.text.setString("Food Eaten: 0");
	ScoreText.text.setCharacterSize(60);
	ScoreText.SetLocalPosition(50, 25);

	home.SetChildrenActive(true, true);
	end.SetActiveHierarchy(false, false);
	ScoreText.SetActiveHierarchy(false, false);
}

void GameGui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Gui::draw(target, states);

}

void GameGui::SetScore(int score)
{
	ScoreText.text.setString("Food Eaten: " + std::to_string(score));
}
