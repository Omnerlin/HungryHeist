#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Player.h>
#include <Utils.h>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>

void ReadJson(Player& player) {
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream config("config.json");
    std::stringstream configBuffer;
    configBuffer << config.rdbuf();

    try {
        nlohmann::json json;
        json = nlohmann::json::parse(configBuffer.str());
        float collisionSizeX = json["playerCollisionSizeX"].get<float>();
        float collisionSizeY = json["playerCollisionSizeY"].get<float>();

        player.collisionRect.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
        player.collisionRect.setOrigin(player.collisionRect.getSize().x / 2,
                    player.collisionRect.getSize().y);
        player.setOrigin(player.getTexture()->getSize().x / 2,
                         player.getTexture()->getSize().y);
        player.groundAcceleration =
            json["playerGroundAcceleration"].get<float>();
        player.friction = json["playerFriction"].get<float>();
        player.maxSpeedX = json["playerMaxSpeedX"].get<float>();
        player.maxSpeedY = json["playerMaxSpeedY"].get<float>();
        player.gravity = json["playerGravity"].get<float>();
        player.jumpForce = json["playerJumpForce"].get<float>();
        player.airDrag = json["playerAirDrag"].get<float>();
        player.floorOffset = json["playerFloorOffset"].get<float>();
    } catch (const std::exception& e) {
        std::cout << "Error opening config file." << std::endl;
        std::cerr << e.what() << std::endl;
    }
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

    // Background :)
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
    sf::RectangleShape collisionRect;
    collisionRect.setSize(sf::Vector2f(32, 32));
    collisionRect.setFillColor(sf::Color(141, 81, 0, 255));
    collisionRect.setOutlineColor(sf::Color::Black);
    collisionRect.setPosition(0, -74);

    ReadJson(player);

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

        // Test collision for player
        if(collisionRect.getGlobalBounds().intersects(player.collisionRect.getGlobalBounds())) {
            if(player.velocity.y < 0 && player.prevPosition.y > (collisionRect.getPosition().y + collisionRect.getSize().y + player.collisionRect.getSize().y)) {
                // We hit the bottom
                player.velocity.y = 0;
                player.setPosition(player.getPosition().x, collisionRect.getPosition().y + collisionRect.getSize().y + player.collisionRect.getSize().y);
            }
            else if(player.velocity.y > 0 && player.prevPosition.y <= (collisionRect.getPosition().y)) {
                // We hit the top
                player.grounded = true;
                player.velocity.y = 0;
                player.setPosition(player.getPosition().x, collisionRect.getPosition().y);
            }
            else if(player.velocity.x > 0 && (player.prevPosition.x + player.collisionRect.getSize().x / 2) <= collisionRect.getPosition().x) {
                // We hit the left side
                player.velocity.x = 0;
                player.setPosition(collisionRect.getPosition().x - player.collisionRect.getSize().x / 2, player.getPosition().y);
            }
            else if(player.velocity.x < 0 && (player.prevPosition.x - player.collisionRect.getSize().x / 2) >= collisionRect.getPosition().x + collisionRect.getSize().x) {
                // We hit the right side
                player.velocity.x = 0;
                player.setPosition(collisionRect.getPosition().x + collisionRect.getSize().x + player.collisionRect.getSize().x / 2, player.getPosition().y);
            }            
        }

        player.collisionRect.setPosition(player.getPosition());
        player.collisionRect.setFillColor(player.grounded ? sf::Color::Green : sf::Color::Red);

        //mainCamera.setCenter(lerp(mainCamera.getCenter().x, player.getPosition().x, 1.6 * deltaTime), lerp(mainCamera.getCenter().y, player.getPosition().y + -32, 1.6 * deltaTime));
        window.setView(mainCamera);
        window.draw(background);
        window.draw(player.collisionRect);
        window.draw(player);
        window.draw(collisionRect);
        window.display();
    }

    return 0;
}