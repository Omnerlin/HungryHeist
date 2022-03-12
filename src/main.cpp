#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Collider.h>
#include <Hand.h>
#include <Physics.h>
#include <Player.h>
#include <Utils.h>

#include <algorithm>
#include <iostream>
#include <random>

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

int main() {
    float timeBetweenHands = 1.f;
    float timeSinceLastHand = 0.f;
    Collider* captureCollider;

    // Create Window
    sf::Vector2f referenceResolution(256, 144);  // Reference Resolution for pixel art.
    sf::Vector2f guiResolution(1280, 720);       // Resolution for GUI layer

    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Janky Game");  // Window Size
    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::View gameWorldCamera(sf::Vector2f(0.f, -referenceResolution.y / 2.f),
                             sf::Vector2f(referenceResolution.x, referenceResolution.y));

    // sf::RenderTexture gameRenderTexture;
    // if (!gameRenderTexture.create(1280, 720)) {
    //     std::cout << "ERROR: Could not create game render texture!" << std::endl;
    // }
    // sf::Texture gameTexture = gameRenderTexture.getTexture();
    //::Sprite gameSprite(gameRenderTexture.getTexture());

    // sf::View guiCamera(sf::Vector2f(guiResolution.x, guiResolution.y) / 2.f,
    //                    sf::Vector2f(guiResolution.x, guiResolution.y));

    sf::Clock deltaClock;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> sideDistribution(0, 1);
    std::uniform_int_distribution<int> topDistribution(2, 3);
    std::uniform_int_distribution<int> colorDistribution(0, 4);

    sf::Text performanceText;
    performanceText.setCharacterSize(10);
    performanceText.setFillColor(sf::Color::Black);
    performanceText.setStyle(sf::Text::Bold);

    // Initialize Player
    Player player;
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/Sphynx01.png");
    player.setTexture(playerTexture);
    player.setOrigin(player.getTexture()->getSize().x / 2, player.getTexture()->getSize().y);
    Physics::RegisterCollider(&player.collider);
    player.collider.CollisionStayCallback = [&](Collider* col) { player.ResolveMovementCollision(col); };
    // player.collider.TriggerOverlapBeginCallback = [&captureCollider, &player](Collider* col) {
    //     captureCollider = col;
    //     player.captured = true;
    //     // Physics::DeregisterCollider(&player.collider);
    // };

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
    std::vector<Hand> hands;
    hands.reserve(_handPositions.size());
    std::cout << "Number of hands: " << _handPositions.size();
    for (size_t i = 0; i < _handPositions.size(); i++) {
        Hand hand;
        hand.setOrigin(32, 0);
        hand.grabTrigger.colliderType = ColliderType::Trigger;
        hand.setTexture(handTexture);
        hand.exclamationSprite.setTexture(exclamationTexture);
        hand.setScale(0.5f, 0.5f);
        hand.grabTrigger.rect.setOrigin(sf::Vector2f(25, 0) * hand.getScale().x);
        hand.grabTrigger.rect.setSize(sf::Vector2f(50, 75) * hand.getScale().x);
        hands.push_back(hand);
        Physics::RegisterCollider(&hands[hands.size() - 1].grabTrigger);
    }

    ReadJson(player);
    auto colliders = LoadCollidersFromConfig();
    for (auto& col : colliders) {
        Physics::RegisterCollider(&col);
    }

    sf::Font font;
    font.loadFromFile("assets/fonts/FiraCode.ttf");

    while (window.isOpen()) {
        Input::ClearPressedKeys();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                Input::AddKeyPress(event.key.code);
            }
        }

        sf::Time time = deltaClock.restart();
        float deltaTime = time.asSeconds();
        timeSinceLastHand += deltaTime;
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
                handToAttackWith->setColor(Hand::SkinColors[colorDistribution(generator)]);
                handToAttackWith->Attack((HandSpawnDirection)(spawnPosition.direction < 2 ? sideDistribution(generator)
                                                                                          : topDistribution(generator)),
                                         gameWorldCamera, 0.75f, spawnPosition.offset);
            }
        }

        performanceText.setFont(font);
        performanceText.setPosition(-100, -100);
        performanceText.setString("Performance (ms): " + std::to_string(time.asMilliseconds()));

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
            player.setPosition(0, 0);
            player.captured = false;
            std::cout << "Done." << std::endl;
        }

        if (Input::KeyWasPressed(KeyCode::Escape)) {
            std::cout << "Exiting..." << std::endl;
            window.close();
        }
        if (Input::KeyWasPressed(KeyCode::Space) && player.grounded) {
            player.AddVelocity(0, -player.jumpForce);
        }
        if (Input::KeyWasPressed(KeyCode::H)) {
        }

        if (!player.captured) {
            player.UpdatePosition(deltaTime);
            player.collider.rect.setPosition(player.getPosition());
        } else {
            player.setPosition(captureCollider->rect.getPosition());
        }

        player.grounded = false;
        if (player.getPosition().y >= player.floorOffset) {
            player.setPosition(player.getPosition().x, player.floorOffset);
            player.velocity.y = 0;
            player.grounded = true;
        }

        player.collider.rect.setPosition(player.getPosition());
        player.collider.rect.setFillColor(player.grounded ? sf::Color::Green : sf::Color::Red);

        for (size_t i = 0; i < hands.size(); i++) {
            hands[i].Update(deltaTime);
        }

        Physics::CheckForCollisionsAndTriggerOverlaps();

        // Draw game world
        window.clear(sf::Color::Transparent);
        gameWorldCamera.setCenter(0,0);
        window.setView(gameWorldCamera);
        window.draw(background);
        for (size_t i = 0; i < colliders.size(); i++) {
            window.draw(colliders[i].rect);
        }
        for (size_t i = 0; i < hands.size(); i++) {
            window.draw(hands[i]);
            //window.draw(hands[i].grabTrigger.rect);
        }
        for (size_t i = 0; i < hands.size(); i++) {
            if (hands[i].GetHandState() == Hand::HandState::Warning) {
                window.draw(hands[i].exclamationSprite);
            }
        }
        window.draw(player);
        window.display();
        //sf::Sprite gameSprite = sf::Sprite(window.getTexture());

        // Draw GUI
        //window.clear();
        //gameSprite.setPosition(250, 250);
        //window.draw(gameSprite);
        // window.setView(gameWorldCamera);
        // window.draw(background);
        // for (size_t i = 0; i < colliders.size(); i++) {
        //     window.draw(colliders[i].rect);
        // }
        // for (size_t i = 0; i < hands.size(); i++) {
        //     window.draw(hands[i]);
        //     window.draw(hands[i].grabTrigger.rect);
        // }
        // for (size_t i = 0; i < hands.size(); i++) {
        //     if (hands[i].GetHandState() == Hand::HandState::Warning) {
        //         window.draw(hands[i].exclamationSprite);
        //     }
        // }
        // window.draw(player);
        // window.draw(performanceText);
        window.display();
    }

    return 0;
}