#pragma once
#include <SFML/Graphics.hpp>
struct Player : sf::Sprite {
    // Transform
    float floorOffset {0};

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
    bool facingLeft;
    sf::Vector2f velocity;
    sf::Vector2f prevPosition;
    
    void AddVelocity(const sf::Vector2f& addVelocity);
    void AddVelocity(float x, float y);
    void UpdatePosition(float deltaTime);

    // Collision
    sf::RectangleShape collisionRect;

    float GetWidth() {
        return getTexture()->getSize().x;
    }
    float GetHeight() {
        return getTexture()->getSize().y;
    }
    const sf::Vector2u getSize() {
        return getTexture()->getSize();
    }
};