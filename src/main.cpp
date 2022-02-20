#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Collider.h>
#include <Hand.h>
#include <Player.h>
#include <Utils.h>
#include <random>
#include <algorithm>
#include <iostream>

/*
Hand Layout            --|---|---|---|--
                       |               |
--- = Wall             -               -
| = Hand               |               |
                       --|---|---|---|--
*/
void AttackWithAHand(std::vector<Hand>& hands, sf::View view,
                     const sf::Texture& handTexture, HandSpawnDirection direction, float offset, sf::Color handColor) {
    Hand* handToAttackWith = nullptr;
    for (size_t i = 0; i < hands.size(); i++) {
        if (hands[i].done) {
            handToAttackWith = &hands[i];
            break;
        }
    }
    if (handToAttackWith == nullptr) {
        std::cout << "Spawning a new hand" << std::endl;
        Hand hand;
        hand.setOrigin(32, 0);
        hand.grabTrigger.colliderType = ColliderType::Trigger;
        hand.grabTrigger.rect.setSize(sf::Vector2f(50, 75));
        hand.grabTrigger.rect.setOrigin(25,0);
        hand.setTexture(handTexture);
//        hand.attackFinishedCallbacks.push_back([&](){PrintDone(1);});
        hands.push_back(hand);
        handToAttackWith = &hands[hands.size() - 1];
    }
    handToAttackWith->setColor(handColor);
    handToAttackWith->Attack(direction, view, 0.75f, offset);
}

void ReadJson(Player& player) { player.LoadSettingsFromConfig(); }

int main() {
    // Create Window
    sf::Vector2f referenceResolution(
        256, 144);  // Reference Resolution for pixel art.
    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Graphics");  // Window Size
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::View mainCamera(
        sf::Vector2f(0.f, -referenceResolution.y / 2.f),
        sf::Vector2f(referenceResolution.x, referenceResolution.y));
    window.setView(mainCamera);
    sf::Clock deltaClock;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,4);
    std::uniform_int_distribution<int> colorDistribution(0,5);

    sf::Text performanceText;
    performanceText.setCharacterSize(10);
    performanceText.setFillColor(sf::Color::Black);
    performanceText.setStyle(sf::Text::Bold);

    // Initialize Player
    Player player;
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/Sphynx01.png");
    player.setTexture(playerTexture);
    player.setOrigin(player.getTexture()->getSize().x / 2,
                     player.getTexture()->getSize().y);

    // Background
    sf::RectangleShape background;
    sf::Texture texture;
    texture.loadFromFile("assets/textures/kitchen2.png");
    background.setSize(sf::Vector2f(texture.getSize().x, texture.getSize().y));
    background.setScale(background.getScale().x / 2,
                        background.getScale().y / 2);
    background.setTexture(&texture, true);
    background.setOrigin(background.getSize().x / 2,
                         background.getSize().y - 82);

    // Hands
    sf::Texture handTexture;
    handTexture.loadFromFile("assets/textures/Hand.png");
    std::vector<Hand> hands;

    ReadJson(player);
    auto colliders = LoadCollidersFromConfig();

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
        window.clear();
        sf::Font font;
        font.loadFromFile("assets/fonts/FiraCode.ttf");
        performanceText.setFont(font);
        performanceText.setPosition(-100, -100);
        performanceText.setString("Performance (ms): " + std::to_string(time.asMilliseconds()));

        // Held Keys
        player.running = false;
        if (Input::KeyIsDown(KeyCode::A)) {
            player.running = true;
            if (player.grounded && player.velocity.x > 0) player.velocity.x = 0;
            player.AddVelocity(
                -sf::Vector2f((deltaTime * player.groundAcceleration), 0.f));
            player.facingLeft = false;
        }
        if (Input::KeyIsDown(KeyCode::D)) {
            if (player.grounded && player.velocity.x < 0) player.velocity.x = 0;
            player.running = true;
            player.AddVelocity(
                sf::Vector2f((deltaTime * player.groundAcceleration), 0.f));
            player.facingLeft = true;
        }

        // Pressed Keys
        if (Input::KeyWasPressed(KeyCode::J)) {
            std::cout << "Reloading config file." << std::endl;
            ReadJson(player);
            colliders = LoadCollidersFromConfig();
            std::cout << "Done." << std::endl;
        }

        if (Input::KeyWasPressed(KeyCode::H)) {
            AttackWithAHand(hands, mainCamera, handTexture, (HandSpawnDirection)(distribution(generator)), 64, Hand::SkinColors[colorDistribution(generator)]);
        }

        if (Input::KeyWasPressed(KeyCode::Escape)) {
            std::cout << "Exiting..." << std::endl;
            window.close();
        }
        if (Input::KeyWasPressed(KeyCode::Space) && player.grounded) {
            player.AddVelocity(0, -player.jumpForce);
        }

        player.UpdatePosition(deltaTime);
        player.collisionRect.setPosition(player.getPosition());

        player.grounded = false;
        if (player.getPosition().y >= player.floorOffset) {
            player.setPosition(player.getPosition().x, player.floorOffset);
            player.velocity.y = 0;
            player.grounded = true;
        }

        for (size_t i = 0; i < colliders.size(); i++) {
            colliders[i].ResolveCollisionAgainstPlayer(player);
        }

        player.collisionRect.setPosition(player.getPosition());
        player.collisionRect.setFillColor(player.grounded ? sf::Color::Green
                                                          : sf::Color::Red);

        for (size_t i = 0; i < hands.size(); i++) {
            hands[i].Update(deltaTime);
            hands[i].grabTrigger.ResolveTriggerOverlapAgainstPlayer(player);
        }

        window.setView(mainCamera);
        window.draw(background);
        // window.draw(player.collisionRect);
        for (size_t i = 0; i < colliders.size(); i++) {
            window.draw(colliders[i].rect);
        }
        for (size_t i = 0; i < hands.size(); i++) {
            window.draw(hands[i]);
            window.draw(hands[i].grabTrigger.rect);
        }
        window.draw(player);
        window.draw(performanceText);
        window.display();
    }

    return 0;
}