#include "Game.h"

#include "Assets.h"
#include "Physics.h"
#include "Utils.h"

Game* Game::Instance = nullptr;

Game::Game(sf::RenderWindow* renderWindow) : window(renderWindow)
{
	if (Instance == nullptr)
	{
		Instance = this;
	}
}

Game::~Game()
{
	if (Instance == this)
	{
		Instance = nullptr;
	}
}

void Game::SetGameState(GameState targetState)
{
	if (targetState == currentState) return;
	switch (targetState)
	{
	case GameState::MainMenu:
		gameSaturation = 1;
		gui.SetGuiState(GameGuiState::Home);
		ResetHands();
		break;

	case GameState::FirstPizza:
		gameSaturation = 1;
		gui.SetScore(0);
		SetGameState(GameState::Gameplay);
		ResetHands();
		break;

	case GameState::Gameplay:
		gui.SetGuiState(GameGuiState::Play);
		break;

	case GameState::End:
		gameSaturation = 0;
		gui.SetGuiState(GameGuiState::End);
		gameSprite.setColor(sf::Color::Red);
		player.captured = true;
		player.animator.SetState("Sad");
		player.transform.SetParent(&captureCollider->transform);
		player.transform.SetWorldRotation(captureCollider->transform.GetWorldRotation());
		player.transform.SetLocalPosition(0, 28);
		player.transform.SetLocalScale(1, 1);
		for (auto& hand : handSpawner.hands) {
			if (&hand.grabTrigger == captureCollider)
			{
				hand.capturedPlayer = true;
				hand.SetHandState(Hand::HandState::Waiting);
			}
			else
			{
				hand.SetHandState(Hand::HandState::Retreating);
			}
		}
		break;

	case GameState::None: 
		break;
	}
	currentState = targetState;
}

void Game::Initialize()
{
	ApplySettingsFromJson();

	gameFont = &Assets::LoadFont("assets/fonts/LilitaOne.ttf");

	// Rendering Setup
	mainCamera = sf::View(sf::Vector2f(0.f, -settings.referenceResolution.y / 2.f),
		sf::Vector2f(settings.referenceResolution.x, settings.referenceResolution.y));
	guiCamera = window->getDefaultView();

	saturationShader.loadFromFile("assets/shaders/saturation.frag", sf::Shader::Fragment);
	saturationShader.setParameter("texture", sf::Shader::CurrentTexture);

	if (!gameRenderTexture.create(settings.screenWidth, settings.screenHeight)) {
		std::cout << "ERROR: Couldn't create game render texture!" << std::endl;
	}
	gameSprite = sf::Sprite(gameRenderTexture.getTexture());

	// Initialize Game Objects
	handSpawner.Initialize();
	player.Initialize();
	foodItem.Initialize();
	background.setTexture(&Assets::LoadTexture("assets/textures/kitchen2.png"));

	player.collider.TriggerOverlapBeginCallback.push_back([this](Collider* col) { 	// Add callback for player collision with food.
		FoodItem& foodItem = Game::Instance->foodItem;
		if (col == &foodItem.collider) {
			foodItem.AssignRandomType();
			foodItem.transform.SetWorldPosition(-256.f / 2 + foodItemDistribution(randGenerator) * 32, -16 + -foodItemDistribution(randGenerator) * 16);
			foodScore++;
			gui.SetScore(foodScore);
		}
		});

	player.collider.TriggerOverlapBeginCallback.emplace_back([this](Collider* col) { 	// Add callback for player collision with a hand
		if (!player.captured && col != &foodItem.collider) {
			captureCollider = col;
			SetGameState(GameState::End);
		}
		});

	GuiElement root_element;
	root_element.SetColor(sf::Color(255, 255, 255, 0));
	root_element.SetRectSize(guiCamera.getSize().x, guiCamera.getSize().y);
	gui.root = root_element;
	gui.gameFont = gameFont;
	gui.BuildMenus();

	gui.QuitButton.onClick.emplace_back([this]() { window->close(); });
	gui.EndQuitButton.onClick.emplace_back([this]() { window->close(); });
	gui.PlayButton.onClick.emplace_back([this]() { SetGameState(GameState::Gameplay); });
	gui.ReplayButton.onClick.emplace_back([this]() { SetGameState(GameState::Gameplay); });
	gui.HomeButton.onClick.emplace_back([this]() { SetGameState(GameState::MainMenu); });
	gui.SetGuiState(Home);
}

void Game::ApplySettingsFromJson()
{
	std::cout << "Loading Configuration" << std::endl;
	std::ifstream config("config/settings.json");
	std::stringstream configBuffer;
	configBuffer << config.rdbuf();

	try {
		nlohmann::json json = nlohmann::json::parse(configBuffer.str());
		settings.timeBetweenHands = json["timeBetweenHands"].get<float>();
		settings.referenceResolution = sf::Vector2f(json["referenceResolution"][0], json["referenceResolution"][1]);
		settings.screenWidth = json["screenWidth"].get<unsigned int>();
		settings.screenHeight = json["screenHeight"].get<unsigned int>();
		settings.windowTitle = json["windowTitle"].get<std::string>();
		settings.fullscreen = json["fullscreen"].get<bool>();
	}
	catch (const std::exception& e) {
		std::cout << "Error opening config file." << std::endl;
		std::cerr << e.what() << std::endl;
	}



	for (auto& col : colliders) {
		Physics::DeregisterCollider(&col);
	}
	colliders = LoadCollidersFromConfig();
	for (auto& col : colliders) {
		Physics::RegisterCollider(&col);
	}

	//ResetPlayer();
	player.LoadSettingsFromConfig();

	std::cout << "Configuration Loaded." << std::endl;
}

void Game::ResetHands()
{
	for (auto& hand : handSpawner.hands) {
		hand.capturedPlayer = false;
		hand.SetHandState(Hand::HandState::Retreating);
	}
}

void Game::ResetPlayer()
{
	player.captured = false;
	player.transform.SetParent(nullptr);
	player.transform.SetLocalRotation(0);
	player.transform.SetWorldPosition(0, 0);
}

void Game::Tick()
{
	const float deltaTime = deltaClock.restart().asSeconds();

	foodItem.glowSprite.transform.LocateRotateBy(15 * deltaTime);
	const float scaleMod = Lerp(1.00f, 1.25f, (sinf(absoluteClock.getElapsedTime().asSeconds() * 6) + 1) / 2);
	foodItem.foodSprite.transform.SetLocalScale(scaleMod, scaleMod);

	player.animator.Update(deltaTime);
	gui.root.UpdateTransforms();
	gui.UpdateHoveredElementNew();

	Physics::CheckForCollisionsAndTriggerOverlaps();
}

void Game::Render()
{
	// Draw game world to texture
	window->clear();
	gameRenderTexture.clear();
	gameRenderTexture.setView(mainCamera);
	gameRenderTexture.draw(background);
	for (const auto& collider : Physics::colliders)
	{
		if (collider.first->colliderType == ColliderType::Solid)
		{
			gameRenderTexture.draw(*collider.first);
		}
	}
	for (const auto& hand : handSpawner.hands)
	{
		gameRenderTexture.draw(hand);
	}
	gameRenderTexture.draw(player.sprite);
	gameRenderTexture.draw(foodItem);
	// gameRenderTexture.draw(foodItem.collider);
	gameRenderTexture.display();

	saturationShader.setUniform("saturation", gameSaturation);
	window->draw(gameSprite, &saturationShader);
	window->setView(guiCamera);


	window->draw(gui);

	window->setView(window->getDefaultView());
	// Render to window
	window->display();
}
