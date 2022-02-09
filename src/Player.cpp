#include <Player.h>
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

void Player::AddVelocity(float x, float y) { AddVelocity(sf::Vector2f(x, y)); }