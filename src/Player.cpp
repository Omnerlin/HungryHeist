#include <Player.h>
void Player::UpdatePosition(float deltaTime) {
    position += velocity * deltaTime;

    if (position.y < floorOffset) {
        grounded = false;
        // velocity.y += gravity * deltaTime;
        AddVelocity(0, gravity * deltaTime);
    } else {
        position.y = floorOffset;
        velocity.y = 0;
        grounded = true;
    }

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

    rect.setPosition(position);
    rect.setRotation(rotation);
}

void Player::AddVelocity(const sf::Vector2f& addVelocity) {
    velocity.x = std::clamp(velocity.x + addVelocity.x, -maxSpeedX, maxSpeedX);
    velocity.y = std::clamp(velocity.y + addVelocity.y, -maxSpeedY, maxSpeedY);
}

void Player::AddVelocity(float x, float y) { AddVelocity(sf::Vector2f(x, y)); }