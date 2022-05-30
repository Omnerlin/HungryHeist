#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Collider.h>
#include <Hand.h>
#include <FoodItem.h>
#include <Physics.h>
#include <Player.h>
#include <Utils.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <filesystem>
#include "EntityTransform.h"
#include <Gui/Gui.h>


/*
Hand Layout            --|---|---|---|--
					   |               |
--- = Wall             |               |
| = Hand               |               |
					   --|---|---|---|--
*/
// void AttackWithAHand(Hand& hand, sf::View view,
//                      const sf::Texture& handTexture, HandSpawnDirection
//                      direction, float offset, sf::Color handColor) {
//     hand.setColor(handColor);
//     hand.Attack(direction, view, 0.75f, offset);
// }

void ReadJson(Player& player) { player.LoadSettingsFromConfig(); }

int main(int arc, char** argv) {
	float timeBetweenHands = 1.f;
	float timeSinceLastHand = 0.f;
	Collider* captureCollider;
	int foodScore = 0;

	auto dir = std::filesystem::path(argv[0]).parent_path();
	std::cout << "EXE Path: " << dir.string() << std::endl;

	// Create Window
	sf::Vector2f referenceResolution(256, 144);  // Reference Resolution for pixel art.
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Janky Game");  // Window Size
	window.setFramerateLimit(60);
	sf::View mainCamera(sf::Vector2f(0.f, -referenceResolution.y / 2.f),
		sf::Vector2f(referenceResolution.x, referenceResolution.y));
	sf::View guiCamera = window.getDefaultView();
	sf::Clock deltaClock;
	sf::Clock clockSinceStart;

	// Gui
	Gui gui;
	GuiElement root_element;
	root_element.SetDebugColor(sf::Color(255,255,255,50));
	root_element.SetRectSize(guiCamera.getSize().x, guiCamera.getSize().y);
	gui.root = &root_element;

	GuiElement redElement;
	redElement.SetParent(&root_element);
	redElement.SetDebugColor(sf::Color::Red);
	redElement.SetRectSize(1280, 20, false);
	redElement.SetAnchorMax(1, 0.0f);
	redElement.SetLocalPosition(0, 0);

	GuiElement greenElement;
	greenElement.SetParent(&root_element);
	greenElement.SetDebugColor(sf::Color::Green);
	greenElement.SetRectSize(300, 50);
	greenElement.SetPivot({ 0.5f, 0.5f });
	greenElement.SetAnchorMin(0.5f, 0.5f);
	greenElement.SetAnchorMax(0.5f, 0.5f);
	greenElement.SetLocalPosition(1280.f/2, 720.f/2);

	GuiElement yellowElement;
	yellowElement.SetParent(&greenElement);
	yellowElement.SetDebugColor(sf::Color::Yellow);
	yellowElement.SetRectSize(50, 25);
	yellowElement.SetPivot({ 1, 0 });
	yellowElement.SetAnchorMin(0, 0);
	yellowElement.SetAnchorMax(1, 0.f);
	yellowElement.SetLocalPosition(50, 25);

	GuiElement blueElement;
	blueElement.SetParent(&yellowElement);
	blueElement.SetDebugColor(sf::Color::Blue);
	blueElement.SetRectSize(10, 10);
	blueElement.SetAnchorMin(0.5f, 0.5f);
	blueElement.SetAnchorMax(0.5f, 0.5f);
	blueElement.SetLocalPosition(20, 5);

	// Other setup
	sf::RenderTexture gameRenderTexture;
	if (!gameRenderTexture.create(1280, 720)) {
		std::cout << "ERROR: Couldn't create game render texture!" << std::endl;
	}
	sf::Sprite gameSprite(gameRenderTexture.getTexture());

	std::default_random_engine generator;
	std::uniform_int_distribution<int> sideDistribution(0, 1);
	std::uniform_int_distribution<int> topDistribution(2, 3);
	std::uniform_int_distribution<int> colorDistribution(0, 4);
	std::uniform_int_distribution<int> foodItemDistribution(1, 7);

	sf::Font debugFont;
	debugFont.loadFromFile("assets/fonts/FiraCode.ttf");
	sf::Font gameFont;
	gameFont.loadFromFile("assets/fonts/LilitaOne.ttf");

	sf::Text performanceText;
	performanceText.setFont(debugFont);
	performanceText.setCharacterSize(24);
	performanceText.setFillColor(sf::Color::Black);
	performanceText.setStyle(sf::Text::Bold);
	performanceText.setPosition(100, 150);

	sf::Text scoreText;
	scoreText.setFont(gameFont);
	scoreText.setCharacterSize(62);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(50, 50);
	scoreText.setString("Food Eaten: 0");

	std::vector<Hand> hands;

	// Create first food item
	FoodItem::FoodTexture.loadFromFile("assets/textures/food.png");
	FoodItem::GlowTexture.loadFromFile("assets/textures/flare.png");
	FoodItem foodItem;
	foodItem.transform.SetOrigin(8, 8);
	foodItem.collider.drawable.setSize(sf::Vector2f(16, 16));
	foodItem.collider.colliderType = ColliderType::Trigger;
	foodItem.collider.transform.SetOrigin(8, 8);
	foodItem.collider.transform.SetParent(&foodItem.transform);
	foodItem.foodSprite.transform.SetParent(&foodItem.transform);
	foodItem.foodSprite.transform.SetOrigin(8, 8);
	foodItem.glowSprite.transform.SetParent(&foodItem.foodSprite.transform);
	foodItem.glowSprite.transform.SetLocalScale(0.25f, 0.25f);
	foodItem.glowSprite.transform.SetOrigin(256.f / 2, 256.f / 2);
	foodItem.transform.SetWorldPosition(0, -64);
	foodItem.foodSprite.drawable.setTexture(FoodItem::FoodTexture);
	foodItem.glowSprite.drawable.setTexture(FoodItem::GlowTexture);
	foodItem.collider.drawable.setFillColor(sf::Color(255, 255, 255, 100));
	foodItem.AssignType(FoodType::Pizza);
	Physics::RegisterCollider(&foodItem.collider);

	// Initialize Player
	Player player;
	sf::Texture playerTexture;
	playerTexture.loadFromFile("assets/textures/Sphynx01.png");
	player.setTexture(playerTexture);
	player.setOrigin(player.getTexture()->getSize().x / 2, player.getTexture()->getSize().y);
	Physics::RegisterCollider(&player.collider);
	player.collider.CollisionStayCallback.push_back([&](Collider* col) { player.ResolveMovementCollision(col); });
	player.collider.TriggerOverlapBeginCallback.push_back([&foodItem, &player, &foodItemDistribution, &generator, &scoreText, &foodScore](Collider* col) {
		if (col == &foodItem.collider) {
			foodItem.AssignRandomType();
			foodItem.transform.SetWorldPosition(-256.f / 2 + foodItemDistribution(generator) * 32, -16 + -foodItemDistribution(generator) * 16);
			foodScore++;
			scoreText.setString("Food Eaten: " + std::to_string(foodScore));
		}
		});
	player.collider.TriggerOverlapBeginCallback.push_back([&captureCollider, &player, &gameSprite, &hands, &foodItem](Collider* col) {
		if (!player.captured && col != &foodItem.collider) {
			captureCollider = col;
			player.captured = true;
			gameSprite.setColor(sf::Color::Red);
			for (auto& hand : hands) {
				hand.SetHandState(Hand::HandState::Retreating);
			}
		}
		});

	// Background
	sf::RectangleShape background;
	sf::Texture texture;
	texture.loadFromFile("assets/textures/kitchen2.png");
	background.setSize(sf::Vector2f(texture.getSize().x, texture.getSize().y));
	background.setScale(background.getScale().x / 2, background.getScale().y / 2);
	background.setTexture(&texture, true);
	background.setOrigin(background.getSize().x / 2, background.getSize().y - 82);

	// Hands
	std::vector<HandSpawnPosition> _handPositions{
		// Bottom row
		HandSpawnPosition{HandSpawnDirection::FromBottom, 32 - 8},
		HandSpawnPosition{HandSpawnDirection::FromBottom, 96 - 20},
		HandSpawnPosition{HandSpawnDirection::FromBottom, 160 - 32},
		HandSpawnPosition{HandSpawnDirection::FromBottom, 224 - 44},
		HandSpawnPosition{HandSpawnDirection::FromBottom, 288 - 56},
		// Right Row
		HandSpawnPosition{HandSpawnDirection::FromRight, 32 - 16},
		HandSpawnPosition{HandSpawnDirection::FromRight, 96 - 24},
		HandSpawnPosition{HandSpawnDirection::FromRight, 160 - 32},
	};

	std::vector<HandSpawnPosition> inactiveHandPositions(_handPositions.size());

	sf::Texture handTexture;
	sf::Texture exclamationTexture;
	handTexture.loadFromFile("assets/textures/Hand.png");
	exclamationTexture.loadFromFile("assets/textures/Exclamation.png");
	hands.reserve(_handPositions.size());
	std::cout << "Number of hands: " << _handPositions.size();
	for (size_t i = 0; i < _handPositions.size(); i++) {
		Hand hand;
		hands.push_back(hand);
		Hand& newHand = hands[hands.size() - 1];
		newHand.grabTrigger.colliderType = ColliderType::Trigger;
		newHand.transform.SetWorldScale(0.5f, 0.5f);
		newHand.grabTrigger.drawable.setSize(sf::Vector2f(50, 75) * newHand.transform.GetWorldScale().x);
		newHand.handSprite.setTexture(handTexture);
		newHand.exclamationSprite.setTexture(exclamationTexture);
		//Physics::RegisterCollider(&newHand.grabTrigger);
		newHand.transform.SetAttachedTransformable(&newHand.handSprite);
		newHand.transform.SetOrigin(32, 0);
		newHand.grabTrigger.transform.SetParent(&newHand.transform);
		newHand.grabTrigger.transform.SetLocalPosition(0, 0);
	}

	ReadJson(player);
	auto colliders = LoadCollidersFromConfig();
	for (auto& col : colliders) {
		Physics::RegisterCollider(&col);
	}

	while (window.isOpen()) {
		Input::ClearPressedKeys();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized) {
				float targetAspect = 16.f / 9;
				float currentImageAspect = (float)event.size.width / event.size.height;
				guiCamera.setSize(sf::Vector2f(window.getSize()));
				guiCamera.setCenter(guiCamera.getSize() / 2.f);

				sf::FloatRect newViewportSettings(0,0,1,1);
				if (currentImageAspect < targetAspect) { // Our window is 
					newViewportSettings = sf::FloatRect(0, (1.f - currentImageAspect / targetAspect) / 2, 1, currentImageAspect / targetAspect);
					gui.root->SetLocalPosition(0, guiCamera.getSize().y * ((1.f - currentImageAspect / targetAspect) / 2));
				}
				else if (currentImageAspect > targetAspect) { // Our window is wider than our game view
					newViewportSettings = sf::FloatRect((1.f - targetAspect / currentImageAspect) / 2, 0, targetAspect / currentImageAspect, 1);
					gui.root->SetLocalPosition(guiCamera.getSize().x * ((1.f - targetAspect / currentImageAspect) / 2), 0);
				}
				mainCamera.setViewport(newViewportSettings);
				gui.root->SetRectSize(guiCamera.getSize().x * newViewportSettings.width, guiCamera.getSize().y * newViewportSettings.height);
			}
			if (event.type == sf::Event::KeyPressed) {
				Input::AddKeyPress(event.key.code);
			}
			gui.ProcessEvent(event);
		}

		sf::Time time = deltaClock.restart();
		float deltaTime = time.asSeconds();
		if (!player.captured) {
			timeSinceLastHand += deltaTime;
		}
		if (timeSinceLastHand >= timeBetweenHands) {
			timeSinceLastHand -= timeBetweenHands;
			if (_handPositions.size() > 0) {
				auto indexDistribution = std::uniform_int_distribution<int>(0, _handPositions.size() - 1);
				int positionIndex = indexDistribution(generator);
				int handIndex = 0;
				for (size_t i = 0; i < hands.size(); i++) {
					if (hands[i].done) {
						handIndex = i;
						break;
					}
				}

				HandSpawnPosition spawnPosition = _handPositions[positionIndex];
				_handPositions.erase(_handPositions.begin() + positionIndex);
				Hand* handToAttackWith = &hands[handIndex];

				handToAttackWith->HandFinishCallback = [&_handPositions, spawnPosition]() {
					_handPositions.push_back(spawnPosition);
				};
				handToAttackWith->handSprite.setColor(Hand::SkinColors[colorDistribution(generator)]);
				handToAttackWith->Attack((HandSpawnDirection)(static_cast<int>(spawnPosition.direction) < 2 ? sideDistribution(generator)
					: topDistribution(generator)),
					mainCamera, 0.75f, spawnPosition.offset);
			}
		}

		// Held Keys
		player.running = false;
		if (Input::KeyIsDown(KeyCode::A)) {
			player.running = true;
			if (player.grounded && player.velocity.x > 0) player.velocity.x = 0;
			player.AddVelocity(-sf::Vector2f((deltaTime * player.groundAcceleration), 0.f));
			player.facingLeft = false;
		}
		if (Input::KeyIsDown(KeyCode::D)) {
			if (player.grounded && player.velocity.x < 0) player.velocity.x = 0;
			player.running = true;
			player.AddVelocity(sf::Vector2f((deltaTime * player.groundAcceleration), 0.f));
			player.facingLeft = true;
		}

		// Pressed Keys
		if (Input::KeyWasPressed(KeyCode::J)) {
			std::cout << "Reloading config file." << std::endl;
			ReadJson(player);
			for (auto& col : colliders) {
				Physics::DeregisterCollider(&col);
			}
			colliders = LoadCollidersFromConfig();
			for (auto& col : colliders) {
				Physics::RegisterCollider(&col);
			}
			player.captured = false;
			player.setPosition(0, 0);
			gameSprite.setColor(sf::Color::White);
			std::cout << "Done." << std::endl;
		}

		if (Input::KeyWasPressed(KeyCode::Escape)) {
			std::cout << "Exiting..." << std::endl;
			window.close();
		}
		if (Input::KeyWasPressed(KeyCode::Space) && player.grounded) {
			player.AddVelocity(0, -player.jumpForce);
		}

		if (!player.captured) {
			player.UpdatePosition(deltaTime);
			player.collider.transform.SetWorldPosition(player.getPosition());
		}
		else {
			player.setPosition(captureCollider->transform.GetWorldPosition());
		}

		performanceText.setString("Performance (ms): " + std::to_string(time.asMilliseconds()));
		player.collider.transform.SetWorldPosition(player.getPosition());

		player.grounded = false;
		if (player.getPosition().y >= player.floorOffset) {
			player.setPosition(player.getPosition().x, player.floorOffset);
			player.velocity.y = 0;
			player.grounded = true;
		}

		player.collider.transform.SetWorldPosition(player.getPosition());

		for (size_t i = 0; i < hands.size(); i++) {
			hands[i].Update(deltaTime);
		}

		foodItem.glowSprite.transform.LocateRotateBy(15 * deltaTime);
		float scaleMod = Lerp(1.00f, 1.25f, (sinf(clockSinceStart.getElapsedTime().asSeconds() * 6) + 1) / 2);
		foodItem.foodSprite.transform.SetLocalScale(scaleMod, scaleMod);

 		Physics::CheckForCollisionsAndTriggerOverlaps();

		// Draw game world to texture
		window.clear();
		gameRenderTexture.clear();
		gameRenderTexture.setView(mainCamera);
		gameRenderTexture.draw(background);
		for (size_t i = 0; i < colliders.size(); i++) {
			gameRenderTexture.draw(colliders[i]);
		}
		for (size_t i = 0; i < hands.size(); i++) {
			gameRenderTexture.draw(hands[i]);
		}
		gameRenderTexture.draw(player);
		gameRenderTexture.draw(foodItem);
		// gameRenderTexture.draw(foodItem.collider);
		gameRenderTexture.display();
		window.draw(gameSprite);
		window.setView(guiCamera);

		gui.root->UpdateTransforms();
		gui.UpdateHoveredElement();
		window.draw(gui);
		window.draw(scoreText);

		window.setView(window.getDefaultView());
		// Render to window
		window.display();
	}

	return 0;
}