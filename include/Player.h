#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

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
    bool captured {false};
    sf::Vector2f velocity;
    sf::Vector2f prevPosition;
    
    void AddVelocity(const sf::Vector2f& addVelocity);
    void AddVelocity(float x, float y);
    void UpdatePosition(float deltaTime);
    void LoadSettingsFromConfig();
    void ResolveMovementCollision(Collider* other);

    // Collision
    Collider collider;

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

