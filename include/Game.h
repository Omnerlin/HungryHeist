#pragma once
#include "Hand.h"
#include "Player.h"
#include "FoodItem.h"
#include "Gui/GameGui.h"
#include "HandSpawner.h"

enum class GameState
{
	None,
	Settings,
	MainMenu,
	FirstPizza,
	MonsterScream,
	Gameplay,
	End
};

struct GameSettings
{
	float timeBetweenHands{ 1.f };
	float referenceResolutionX{ 256};  // Reference Resolution for pixel art.
	float referenceResolutionY{ 144 };  // Reference Resolution for pixel art.
	unsigned int screenWidth{ 1280 };
	unsigned int screenHeight{ 720 };
	std::string windowTitle{"Janky Game"};
	bool fullscreen{ false };
};

struct Game
{
	Game(sf::RenderWindow* renderWindow);
	~Game();
	static Game* Instance;

	// Higher-level management Structs
	GameSettings settings;
	sf::RenderWindow* window{ nullptr };
	sf::Sprite gameSprite;
	sf::RenderTexture gameRenderTexture;
	GameState currentState{ GameState::None };
	int foodScore{ 0 };

	// Game Objects
	GuiElement root_element;
	sf::RectangleShape background;
	Player player;
	FoodItem foodItem;
	GameGui gui;
	HandSpawner handSpawner;
	Collider* captureCollider{nullptr};
	std::vector<Collider> colliders;
	sf::Texture backgroundTexture;

	// Views
	sf::View mainCamera;
	sf::View guiCamera;

	// Time
	sf::Clock deltaClock;
	sf::Clock absoluteClock;

	// Randomness
	std::default_random_engine randGenerator;
	std::uniform_int_distribution<int> foodItemDistribution{ 1, 7 };
	siv::PerlinNoise::seed_type shakeSeed = 123456u;
	siv::PerlinNoise camNoise = siv::PerlinNoise(Hand::seed);

	// Sound
	std::vector<sf::Sound> munchSounds;
	sf::Sound choirSound;

	// Other
	float gameSaturation{ 1.f };
	sf::Shader saturationShader;
	sf::Font* gameFont;

	// Intro Cinematic
	float monsterScreamTime{ 2.f };
	float monsterScreamTimeLeft;
	float monsterTransitionTime{ 1.6f };
	float monsterTransitionTimeLeft;
	float lightSwitchDelay{ 0.75f };
	float lightSwitchTimeLeft;
	bool playDoor;
	sf::Sound lightSwitchSound;
	sf::Sound monsterSound;
	sf::Sound rumbleSound;
	sf::Sound doorSound;
	sf::Sound footstepSound;

	// Methods
	void SetGameState(GameState targetState);
	void Initialize();
	void ApplySettingsFromJson();
	void ResetHands();
	void ResetPlayer();
	void Tick();
	void Render();

};
