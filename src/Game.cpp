#include "Game.h"

#include "Assets.h"
#include "Input.h"
#include "Physics.h"
#include "Utils.h"
#include "SFML/Audio/Listener.hpp"

Game* Game::Instance = nullptr;
std::vector<GameSound*> Game::registeredSounds;
std::vector<GameMusic*> Game::registeredMusic;
std::filesystem::path Game::rootPath;

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
	GameState previousState = currentState;
	if (targetState == currentState) return;

	switch (targetState)
	{
	case GameState::MainMenu:
		StopAllSounds();
		background.setFillColor(sf::Color::White);
		handSpawner.ResetHands();
		handSpawner.handSpawningEnabled = false;
		gameSaturation = 1;
		gui.SetGuiState(GameGuiState::Home);
		player.inputEnabled = false;
		foodItem.transform.SetWorldScale(0, 0);
		foodItem.collider.enabled = false;
		ResetHands();
		ResetPlayer();
		mainMenuMusic.SetBaseVolume(50);
		mainMenuMusic.play();
		break;

	case GameState::FirstPizza:
		foodScore = 0;
		StopAllSounds();
		handSpawner.ResetHands();
		handSpawner.handSpawningEnabled = false;
		gameplayMusic.stop();
		choirSound.play();
		prePizzaMusic.SetBaseVolume(100);
		prePizzaMusic.play();
		mainMenuMusic.stop();
		gameSaturation = 1;
		gui.SetScore(0);
		gui.SetGuiState(GameGuiState::Play);
		player.inputEnabled = true;
		background.setFillColor(sf::Color(100, 100, 100, 255));
		foodItem.transform.SetWorldPosition(0, -64);
		foodItem.transform.SetWorldScale(1, 1);
		foodItem.collider.enabled = true;

		if (previousState != GameState::MainMenu)
		{
			ResetHands();
			ResetPlayer();
		}
		break;

	case GameState::MonsterScream:
		monsterScreamTimeLeft = monsterScreamTime;
		monsterTransitionTimeLeft = monsterTransitionTime;
		lightSwitchTimeLeft = lightSwitchDelay;
		playDoor = true;
		break;

	case GameState::Gameplay:
		handSpawner.handSpawningEnabled = true;
		gameSaturation = 1;
		gui.SetGuiState(GameGuiState::Play);
		//if(previousState != GameState::MonsterScream)
		//{
		//	gui.SetScore(0);
		//	ResetHands();
		//	ResetPlayer();
		//}
		player.inputEnabled = true;
		break;

	case GameState::End:
		recordScratch.play();
		gameplayMusic.stop();
		sadMusic.play();
		handSpawner.handSpawningEnabled = false;
		player.inputEnabled = false;
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

	paused = false;
	currentState = targetState;
}

void Game::Initialize()
{

	handSpawner.Initialize();
	player.Initialize();
	foodItem.Initialize();
	ApplySettingsFromJson();
	// Initialize Game Objects
	background.setTexture(&Assets::LoadTexture("assets/textures/kitchen2.png"));
	background.setSize(sf::Vector2f(background.getTexture()->getSize().x, background.getTexture()->getSize().y));
	background.setScale(background.getScale().x / 2, background.getScale().y / 2);
	background.setOrigin(background.getSize().x / 2, background.getSize().y - 82);

	gameFont = &Assets::LoadFont("assets/fonts/LilitaOne.ttf");

	// Rendering Setup
	mainCamera = sf::View(sf::Vector2f(0.f, -settings.referenceResolutionY / 2.f),
		sf::Vector2f(settings.referenceResolutionX, settings.referenceResolutionY));

	auto videoSize = /*settings.fullscreen ? sf::VideoMode::getFullscreenModes()[0] : */sf::VideoMode(settings.screenWidth, settings.screenHeight);
	guiCamera = sf::View(sf::Vector2f(0.5f * videoSize.width, 0.5f * videoSize.height), sf::Vector2f(videoSize.width, videoSize.height));

	saturationShader.loadFromFile(Game::GetAbsolutePath("assets/shaders/saturation.frag"), sf::Shader::Fragment);
	saturationShader.setParameter("texture", sf::Shader::CurrentTexture);

	if (!gameRenderTexture.create(settings.screenWidth, settings.screenHeight)) {
		std::cout << "ERROR: Couldn't create game render texture!" << std::endl;
	}
	gameSprite = sf::Sprite(gameRenderTexture.getTexture());

	player.collider.TriggerOverlapBeginCallback.emplace_back([this](Collider* col) { 	// Add callback for player collision with food.
		if (col == &foodItem.collider) {
			foodItem.AssignRandomType();
			foodItem.transform.SetWorldPosition(-256.f / 2 + foodItemDistribution(randGenerator) * 32, -16 + -foodItemDistribution(randGenerator) * 16);
			std::uniform_int_distribution<int> foodSoundDistribution(0, 2);
			munchSounds[foodSoundDistribution(randGenerator)].play();
			foodScore++;
			gui.SetScore(foodScore);
			if (currentState == GameState::FirstPizza)
			{
				SetGameState(GameState::MonsterScream);
			}
		}
		});

	player.collider.TriggerOverlapBeginCallback.emplace_back([this](Collider* col) { 	// Add callback for player collision with a hand
		if (!player.captured && col != &foodItem.collider) {
			captureCollider = col;
			SetGameState(GameState::End);
		}
		});

	// Sounds
	choirSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/choir.wav"));
	rumbleSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/rumble.wav"));
	lightSwitchSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/Switches/switch3.wav"));
	//GameSound munch0, munch1, munch2;
	//munch0.setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_01.ogg"));
	//munch1.setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_03.ogg"));
	//munch2.setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_04.ogg"));
	munchSounds.reserve(3);
	munchSounds.emplace_back(GameSound());
	munchSounds.emplace_back(GameSound());
	munchSounds.emplace_back(GameSound());
	munchSounds[0].setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_01.ogg"));
	munchSounds[1].setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_03.ogg"));
	munchSounds[2].setBuffer(Assets::LoadSoundBuffer("assets/sounds/eat_04.ogg"));
	rumbleSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/rumble.wav"));
	monsterSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/roar.wav"));
	doorSound.setBuffer(Assets::LoadSoundBuffer("assets/sounds/doorOpen2.wav"));
	recordScratch.setBuffer(Assets::LoadSoundBuffer("assets/sounds/recordScratch.wav"));

	gameplayMusic.openFromFile(GetAbsolutePath("assets/music/gameplayMusic.wav"));
	gameplayMusic.setLoop(true);
	gameplayMusic.SetBaseVolume(50);
	mainMenuMusic.openFromFile(GetAbsolutePath("assets/music/forestLoop.ogg"));
	mainMenuMusic.setLoop(true);
	prePizzaMusic.openFromFile(GetAbsolutePath("assets/music/preEatMusic.wav"));
	prePizzaMusic.setLoop(true);
	sadMusic.openFromFile(GetAbsolutePath("assets/music/sadPiano.wav"));
	sadMusic.setLoop(true);

	// GUI
	root_element.SetColor(sf::Color(255, 255, 255, 0));
	root_element.SetRectSize(guiCamera.getSize().x, guiCamera.getSize().y);
	gui.root = &root_element;
	gui.gameFont = gameFont;
	gui.BuildMenus();
	gui.quitButton.onClick.emplace_back([this]() { window->close(); });
	gui.endQuitButton.onClick.emplace_back([this]() { window->close(); });
	gui.playButton.onClick.emplace_back([this]() { SetGameState(GameState::FirstPizza); });
	gui.replayButton.onClick.emplace_back([this]() { SetGameState(GameState::FirstPizza); });
	gui.homeButton.onClick.emplace_back([this]() { SetGameState(GameState::MainMenu); });
	gui.settingsButton.onClick.emplace_back([this]() { gui.SetGuiState(GameGuiState::Settings); });
	gui.settingsReturnButton.onClick.emplace_back([this]() { SaveJsonSettings(); gui.SetGuiState(GameGuiState::Home); });
	HandleResize({ settings.screenWidth, settings.screenHeight });
	gameSprite.setScale((float)window->getSize().x / settings.screenWidth,
		(float)window->getSize().y / settings.screenHeight);


	gui.fullscreenToggle.onToggleChanged = [this](bool bol) {settings.fullscreen = bol;  window->create(settings.fullscreen ? sf::VideoMode::getFullscreenModes()[0] : sf::VideoMode(settings.screenWidth, settings.screenHeight),
		settings.windowTitle, settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	window->setFramerateLimit(60);
	HandleResize({ window->getSize().x, window->getSize().y });
	gameSprite.setScale((float)window->getSize().x / settings.screenWidth,
		(float)window->getSize().y / settings.screenHeight);
	};
	gui.volumeSlider.sliderValueChanged = [this](float value) { settings.masterVolume = value; SetMasterVolume(value); };


	gui.volumeSlider.SetValue(settings.masterVolume, true);

	SetGameState(GameState::MainMenu);
}

void Game::ApplySettingsFromJson()
{
	std::cout << "Loading Configuration" << std::endl;
	std::ifstream config(GetAbsolutePath("config/settings.json"));
	std::stringstream configBuffer;
	configBuffer << config.rdbuf();

	try {
		nlohmann::json json = nlohmann::json::parse(configBuffer.str());
		settings.timeBetweenHands = json["timeBetweenHands"].get<float>();
		settings.referenceResolutionX = json["referenceResolutionX"].get<float>();
		settings.referenceResolutionY = json["referenceResolutionY"].get<float>();
		settings.screenWidth = json["screenWidth"].get<unsigned int>();
		settings.screenHeight = json["screenHeight"].get<unsigned int>();
		settings.windowTitle = json["windowTitle"].get<std::string>();
		settings.fullscreen = json["fullscreen"].get<bool>();
		settings.masterVolume = json["masterVolume"].get<float>();
	}
	catch (const std::exception& e) {
		std::cout << "Error opening game config file." << e.what() << std::endl;
		std::cerr << e.what() << std::endl;
	}

	
	window->create(settings.fullscreen ? sf::VideoMode::getFullscreenModes()[0] : sf::VideoMode(settings.screenWidth, settings.screenHeight),
		settings.windowTitle, settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	window->setFramerateLimit(60);


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

void Game::SaveJsonSettings()
{
	nlohmann::json settingsSave;

	settingsSave["timeBetweenHands"] = settings.timeBetweenHands;
	settingsSave["referenceResolutionX"] = settings.referenceResolutionX;
	settingsSave["referenceResolutionY"] = settings.referenceResolutionY;
	settingsSave["screenWidth"] = settings.screenWidth;
	settingsSave["screenHeight"] = settings.screenHeight;
	settingsSave["windowTitle"] = settings.windowTitle;
	settingsSave["fullscreen"] = settings.fullscreen;
	settingsSave["masterVolume"] = settings.masterVolume;

	std::ofstream config(GetAbsolutePath("config/settings.json"));
	config << settingsSave;
	config.close();
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
	player.velocity = sf::Vector2f(0, 0);
}

void Game::Tick()
{
	float deltaTime = deltaClock.restart().asSeconds();
	if(deltaTime > 1.f / 20.f)
	{
		deltaTime = 1.f / 20.f;
	}

	if (currentState != GameState::MainMenu && currentState != GameState::End && Input::KeyWasPressed(KeyCode::Escape))
	{
		paused = !paused;
		if (paused)
		{
			gui.SetGuiState(GameGuiState::Pause);
		}
		else
		{
			gui.SetGuiState(GameGuiState::Play);
		}
	}

	if (!paused) {


		if (currentState == GameState::FirstPizza)
		{
			mainCamera.setSize(LerpVector(mainCamera.getSize(), sf::Vector2f{ settings.referenceResolutionX, settings.referenceResolutionY } *0.6f, deltaTime * 5.f));
			mainCamera.setCenter(LerpVector(mainCamera.getCenter(), (player.transform.GetWorldPosition() + foodItem.transform.GetWorldPosition()) / 2.f, deltaTime * 5.f));
		}
		else if (currentState == GameState::End)
		{
			gameSaturation = Lerp(gameSaturation, 0, deltaTime * 2.f);
			mainCamera.setSize(LerpVector(mainCamera.getSize(), sf::Vector2f{ settings.referenceResolutionX, settings.referenceResolutionY } *0.6f, deltaTime * 5.f));
			mainCamera.setCenter(LerpVector(mainCamera.getCenter(), player.transform.GetWorldPosition() + sf::Vector2f(player.transform.GetWorldRotation() < 90 ? -32 : 32, 0), deltaTime * 5.f));
		}
		else
		{
			mainCamera.setSize(LerpVector(mainCamera.getSize(), sf::Vector2f{ settings.referenceResolutionX, settings.referenceResolutionY }, deltaTime * 5.f));
			mainCamera.setCenter(LerpVector(mainCamera.getCenter(), sf::Vector2f(0.f, -settings.referenceResolutionY / 2.f), deltaTime * 5.f));
		}

		if (currentState == GameState::MonsterScream)
		{
			if (monsterTransitionTimeLeft > 0)
			{
				monsterTransitionTimeLeft -= deltaTime;
				if (monsterTransitionTimeLeft > 0)
				{
					prePizzaMusic.SetBaseVolume(Lerp(0, 100, monsterTransitionTimeLeft / monsterTransitionTime));
				}

				if (playDoor && monsterTransitionTimeLeft <= 0.7f)
				{
					doorSound.play();
					playDoor = false;
				}
				if (monsterTransitionTimeLeft <= 0)
				{
					prePizzaMusic.stop();
					lightSwitchSound.play();
					background.setFillColor(sf::Color::White);
				}
			}
			else if (lightSwitchTimeLeft > 0)
			{
				lightSwitchTimeLeft -= deltaTime;
				if (lightSwitchTimeLeft <= 0)
				{
					monsterSound.play();
					rumbleSound.play();
				}
			}
			else
			{
				float intensity = 1.5f;
				float noiseX = camNoise.normalizedOctave1D(monsterScreamTimeLeft * 20, 1);
				float noiseY = camNoise.normalizedOctave1D(monsterScreamTimeLeft * 50, 1);
				mainCamera.setCenter(sf::Vector2f(0.f, -settings.referenceResolutionY / 2.f) + sf::Vector2f(noiseX, noiseY) * intensity);
				monsterScreamTimeLeft -= deltaTime;

				if (monsterScreamTimeLeft <= 0.5f && gameplayMusic.getStatus() != sf::SoundSource::Playing)
				{
					gameplayMusic.play();
				}
				if (monsterScreamTimeLeft <= 0)
				{
					mainCamera.setCenter(sf::Vector2f(0.f, -settings.referenceResolutionY / 2.f));
					rumbleSound.stop();
					SetGameState(GameState::Gameplay);
				}
			}
		}

		player.Update(deltaTime);
		handSpawner.Update(deltaTime);
		Physics::CheckForCollisionsAndTriggerOverlaps();
	}

	foodItem.glowSprite.transform.LocateRotateBy(15 * deltaTime);
	const float scaleMod = Lerp(1.00f, 1.25f, (sinf(absoluteClock.getElapsedTime().asSeconds() * 6) + 1) / 2);
	foodItem.foodSprite.transform.SetLocalScale(scaleMod, scaleMod);
	player.animator.Update(deltaTime);
	gui.title.SetLocalPosition(gui.title.GetLocalPosition().x, (sinf(absoluteClock.getElapsedTime().asSeconds() * 1.5) * 15));

}

void Game::Render()
{
	// Draw game world to texture
	window->clear();
	gameRenderTexture.clear();
	gameRenderTexture.setView(mainCamera);
	gameRenderTexture.draw(background);
	for (const auto& collider : colliders)
	{
		if (collider.draw)
		{
			gameRenderTexture.draw(collider);
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

void Game::StopAllSounds()
{
	gameplayMusic.stop();
	prePizzaMusic.stop();
	mainMenuMusic.stop();
	rumbleSound.stop();
	choirSound.stop();
	monsterSound.stop();
	sadMusic.stop();
}

void Game::HandleResize(const sf::Vector2u& size)
{
	float targetAspect = 16.f / 9;
	float currentImageAspect = (float)size.x / size.y;
	guiCamera.setSize(sf::Vector2f(window->getSize()));
	guiCamera.setCenter(guiCamera.getSize() / 2.f);

	sf::FloatRect newViewportSettings(0, 0, 1, 1);
	if (currentImageAspect < targetAspect) { // Our window is 
		newViewportSettings = sf::FloatRect(0, (1.f - currentImageAspect / targetAspect) / 2, 1, currentImageAspect / targetAspect);
		gui.root->SetLocalPosition(0, guiCamera.getSize().y * ((1.f - currentImageAspect / targetAspect) / 2));
	}
	else if (currentImageAspect > targetAspect) { // Our window is wider than our view
		newViewportSettings = sf::FloatRect((1.f - targetAspect / currentImageAspect) / 2, 0, targetAspect / currentImageAspect, 1);
		gui.root->SetLocalPosition(guiCamera.getSize().x * ((1.f - targetAspect / currentImageAspect) / 2), 0);
	}
	mainCamera.setViewport(newViewportSettings);
	gui.root->SetRectSize(guiCamera.getSize().x * newViewportSettings.width, guiCamera.getSize().y * newViewportSettings.height);
}

void Game::SetMasterVolume(float volume)
{
	settings.masterVolume = volume;
	sf::Listener::setGlobalVolume(volume * 100);
}

std::string Game::GetAbsolutePath(const std::string& path)
{
	return std::filesystem::path(rootPath).append(path).string();
}
