#pragma once
#include "Hand.h"
#include "Player.h"
#include "FoodItem.h"
#include "Gui/GameGui.h"
#include "HandSpawner.h"

enum class GameState
{
	None,
	MainMenu,
	FirstPizza,
	Gameplay,
	End
};

struct GameSettings
{
	float timeBetweenHands{ 1.f };
	sf::Vector2f referenceResolution{ 256, 144 };  // Reference Resolution for pixel art.
	unsigned int screenWidth{ 1280 };
	unsigned int screenHeight{ 720 };
	std::string windowTitle{"Janky Game"};
	bool fullScreen{ false };
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
	sf::RectangleShape background;
	Player player;
	FoodItem foodItem;
	GameGui gui;
	HandSpawner handSpawner;
	Collider* captureCollider{nullptr};
	std::vector<Collider> colliders;

	// Views
	sf::View mainCamera;
	sf::View guiCamera;

	// Time
	sf::Clock deltaClock;
	sf::Clock absoluteClock;

	// Randomness
	std::default_random_engine randGenerator;
	std::uniform_int_distribution<int> foodItemDistribution{ 1, 7 };

	// Other
	float gameSaturation{ 1.f };
	sf::Shader saturationShader;
	sf::Font* gameFont;

	// Methods
	void SetGameState(GameState targetState);
	void Initialize();
	void ApplySettingsFromJson();
	void ResetHands();
	void ResetPlayer();
	void Tick();
	void Render();

};
