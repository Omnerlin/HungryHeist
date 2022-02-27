#include <Player.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>

void Player::LoadSettingsFromConfig() {
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    std::ifstream config("config/player.json");
    std::stringstream configBuffer;
    configBuffer << config.rdbuf();

    try {
        nlohmann::json json;
        json = nlohmann::json::parse(configBuffer.str());
        float collisionSizeX = json["playerCollisionSizeX"].get<float>();
        float collisionSizeY = json["playerCollisionSizeY"].get<float>();
        collider.rect.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
        collider.rect.setOrigin(collider.rect.getSize().x / 2,
                                collider.rect.getSize().y);
        setOrigin(getTexture()->getSize().x / 2, getTexture()->getSize().y);
        groundAcceleration = json["playerGroundAcceleration"].get<float>();
        friction = json["playerFriction"].get<float>();
        maxSpeedX = json["playerMaxSpeedX"].get<float>();
        maxSpeedY = json["playerMaxSpeedY"].get<float>();
        gravity = json["playerGravity"].get<float>();
        jumpForce = json["playerJumpForce"].get<float>();
        airDrag = json["playerAirDrag"].get<float>();
        floorOffset = json["playerFloorOffset"].get<float>();
    } catch (const std::exception& e) {
        std::cout << "Error opening config file." << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void Player::UpdatePosition(float deltaTime) {
    sf::Vector2f currentPosition = getPosition();
    prevPosition = currentPosition;
    AddVelocity(0, gravity * deltaTime);
    currentPosition += velocity * deltaTime;

    if (!running) {
        if (velocity.x > 0) {
            AddVelocity((grounded ? -friction : -airDrag) * deltaTime, 0);
            if (velocity.x <= 0) {
                velocity.x = 0;
            }
        } else if (velocity.x < 0) {
            AddVelocity((grounded ? friction : airDrag) * deltaTime, 0);
            if (velocity.x >= 0) {
                velocity.x = 0;
            }
        }
    }

    setScale(facingLeft ? -1 : 1, 1);
    setPosition(currentPosition);
}

void Player::AddVelocity(const sf::Vector2f& addVelocity) {
    velocity.x = std::clamp(velocity.x + addVelocity.x, -maxSpeedX, maxSpeedX);
    velocity.y = std::clamp(velocity.y + addVelocity.y, -maxSpeedY, maxSpeedY);
}

void Player::ResolveMovementCollision(Collider* other) {
    // Test collision for player{
    if (other->HasCollisionDirectionEnabled(Bottom) && velocity.y < 0 &&
        prevPosition.y >
            (other->rect.getPosition().y + other->rect.getSize().y +
             collider.rect.getSize().y)) {
        // We hit the bottom
        velocity.y = 0;
        setPosition(getPosition().x, other->rect.getPosition().y +
                                         other->rect.getSize().y +
                                         collider.rect.getSize().y);
    } else if (other->HasCollisionDirectionEnabled(Top) && velocity.y > 0 &&
               prevPosition.y <= (other->rect.getPosition().y)) {
        // We hit the top
        grounded = true;
        velocity.y = 0;
        setPosition(getPosition().x, other->rect.getPosition().y);
    } else if (other->HasCollisionDirectionEnabled(Left) && velocity.x > 0 &&
               (prevPosition.x + collider.rect.getSize().x / 2) <=
                   other->rect.getPosition().x) {
        // We hit the left side
        velocity.x = 0;
        setPosition(other->rect.getPosition().x - collider.rect.getSize().x / 2,
                    getPosition().y);
    } else if (other->HasCollisionDirectionEnabled(Right) && velocity.x < 0 &&
               (prevPosition.x - collider.rect.getSize().x / 2) >=
                   other->rect.getPosition().x + other->rect.getSize().x) {
        // We hit the right side
        velocity.x = 0;
        setPosition(other->rect.getPosition().x + other->rect.getSize().x +
                        collider.rect.getSize().x / 2,
                    getPosition().y);
    }
}

void Player::AddVelocity(float x, float y) { AddVelocity(sf::Vector2f(x, y)); }