#pragma once
#include <SFML/Graphics.hpp>
struct Player {
    // Transform
    sf::Vector2f position;
    float rotation;
    float floorOffset {0};

    // Visuals
    sf::RectangleShape rect;

    // Movement
    float groundAcceleration{800.f};
    float friction{50.f};
    float maxSpeedX{200.f};
    float maxSpeedY{1000.f};
    float gravity{800.f};
    float jumpForce{600.f};
    float airDrag{1000.f};
    bool grounded;
    bool running;

    sf::Vector2f velocity;
    void AddVelocity(const sf::Vector2f& addVelocity);
    void AddVelocity(float x, float y);
    void UpdatePosition(float deltaTime);
};