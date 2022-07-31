#pragma once
#include "Hand.h"
#include "Player.h"
#include "FoodItem.h"
#include "GameMusic.h"
#include "GameSound.h"
#include "Gui/GameGui.h"
#include "HandSpawner.h"
#include <filesystem>

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
	float masterVolume{1.f};
	std::string windowTitle{"Janky Game"};
	bool fullscreen{ false };
};

struct Game
{
	Game(sf::RenderWindow* renderWindow);
	~Game();
	static Game* Instance;
	static std::filesystem::path rootPath;

	// Higher-level management Structs
	GameSettings settings;
	sf::RenderWindow* window{ nullptr };
	sf::Sprite gameSprite;
	sf::RenderTexture gameRenderTexture;
	GameState currentState{ GameState::None };
	int foodScore{ 0 };
	bool paused{ false };

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

	// Sound
	std::vector<GameSound> munchSounds;
	GameSound choirSound;
	static std::vector<GameSound*> registeredSounds;
	static std::vector<GameMusic*> registeredMusic;

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
	GameSound lightSwitchSound;
	GameSound monsterSound;
	GameSound rumbleSound;
	GameSound doorSound;
	GameSound footstepSound;
	GameSound recordScratch;

	GameMusic prePizzaMusic;
	GameMusic gameplayMusic;
	GameMusic mainMenuMusic;
	GameMusic sadMusic;

	// Methods
	void SetGameState(GameState targetState);
	void Initialize();
	void ApplySettingsFromJson();
	void SaveJsonSettings();
	void ResetHands();
	void ResetPlayer();
	void Tick();
	void Render();
	void StopAllSounds();
	void HandleResize(const sf::Vector2u& size);
	void SetMasterVolume(float volume);
	void SetFullscreen(bool fullscreen);
	static std::string GetAbsolutePath(const std::string& path);
};
