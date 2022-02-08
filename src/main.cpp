#include <Input.h>

#include <SFML/Graphics.hpp>
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <Player.h>
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
        float playerSizeX = json["playerSizeX"].get<float>();
        float playerSizeY = json["playerSizeY"].get<float>();

        player.rect.setSize(sf::Vector2f(playerSizeX, playerSizeY));
        player.rect.setOrigin(player.rect.getSize().x / 2,
                              player.rect.getSize().y);
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
    sf::Vector2f referenceResolution(16 * 16.f, 16 * 9.f); // Reference Resolution for pixel art.
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Graphics"); // Window Size
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::View mainCamera(sf::Vector2f(0.f, -referenceResolution.y / 2.f),
                        sf::Vector2f(referenceResolution.x, referenceResolution.y));
    window.setView(mainCamera);
    sf::Clock deltaClock;

    // Initialize Player
    Player player;
    player.rect.setSize(sf::Vector2f(32.f, 32.f));
    player.rect.setOrigin(player.rect.getSize().x / 2, player.rect.getSize().y);
    player.position.x = 0;
    player.position.y = 0;
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/Sphynx01.png");
    player.rect.setTexture(&playerTexture);

    // Background :)
    sf::RectangleShape rect;
    sf::Texture texture;
    texture.loadFromFile("assets/textures/kitchen2.png");
    rect.setSize(sf::Vector2f(texture.getSize().x, texture.getSize().y));
    rect.setScale(rect.getScale().x / 2, rect.getScale().y / 2);
    rect.setTexture(&texture, true);
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y - 82);

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
        }
        if (Input::KeyIsDown(KeyCode::D)) {
            if (player.grounded && player.velocity.x < 0) player.velocity.x = 0;
            player.running = true;
            player.AddVelocity(
                sf::Vector2f((deltaTime * player.groundAcceleration), 0.f));
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
        window.draw(rect);
        window.draw(player.rect);
        window.display();
    }

    return 0;
}