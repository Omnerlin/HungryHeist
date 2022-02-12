#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Player.h>
#include <Utils.h>
#include <algorithm>
#include <iostream>
#include <Collider.h>

void ReadJson(Player& player) {
    player.LoadSettingsFromConfig();
}

int main() {
    // Create Window
    sf::Vector2f referenceResolution(
        16 * 16.f, 16 * 9.f);  // Reference Resolution for pixel art.
    sf::RenderWindow window(sf::VideoMode(1280, 720),
                            "Graphics");  // Window Size
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::View mainCamera(
        sf::Vector2f(0.f, -referenceResolution.y / 2.f),
        sf::Vector2f(referenceResolution.x, referenceResolution.y));
    window.setView(mainCamera);
    sf::Clock deltaClock;

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

    // Debug Player
    sf::RectangleShape playerCollision(sf::Vector2f(32, 32));
    playerCollision.setOrigin(player.getOrigin());

    // Test collision


    // Collider colliders[4];
    // for(int i = 0; i < 4; i++) {
    //     if(i == 0) colliders[i].colliderType = ColliderType::Trigger;
    //     colliders[i].rect.setSize(sf::Vector2f(18,18));
    //     colliders[i].rect.setPosition(45 * i - 60, -32);
    //     colliders[i].rect.setFillColor(sf::Color::White);
    // }



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

        float deltaTime = deltaClock.restart().asSeconds();
        window.clear();

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
        if(player.getPosition().y >= player.floorOffset) {
            player.setPosition(player.getPosition().x, player.floorOffset);
            player.velocity.y = 0;
            player.grounded = true;
        }

        for(size_t i = 0; i < colliders.size(); i++) {
            colliders[i].ResolveCollisionAgainstPlayer(player);
        }

        player.collisionRect.setPosition(player.getPosition());
        player.collisionRect.setFillColor(player.grounded ? sf::Color::Green : sf::Color::Red);

        window.setView(mainCamera);
        window.draw(background);
        //window.draw(player.collisionRect);
        for(size_t i = 0; i < colliders.size(); i++) {
            window.draw(colliders[i].rect);
        }
        window.draw(player);
        window.display();
    }

    return 0;
}