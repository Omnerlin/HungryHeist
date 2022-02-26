#include <Collider.h>
#include <Player.h>

bool Collider::HasCollisionDirectionEnabled(CollisionDirection direction) {
    return (direction | collisionDirection) == collisionDirection;
}

void Collider::InvokeInitialCallbackAgainstCollider(Collider* other) {
    if(colliderType == ColliderType::Solid && other->colliderType == ColliderType::Solid) {
        if(other->CollisionBeginCallback != nullptr) {
            other->CollisionBeginCallback(this);
        }
        if(CollisionBeginCallback != nullptr) {
            CollisionBeginCallback(other);
        }
    }
    else {
        // do the same for triggers
        if(colliderType == ColliderType::Trigger && other->TriggerOverlapBeginCallback != nullptr) {
            other->TriggerOverlapBeginCallback(this);
        }
        if(other->colliderType == ColliderType::Trigger && TriggerOverlapBeginCallback != nullptr) {
            TriggerOverlapBeginCallback(other);
        }
    }
}
void Collider::InvokeEndCallbackAgainstCollider(Collider* other) {
    if(colliderType == ColliderType::Solid && other->colliderType == ColliderType::Solid) {
        if(other->CollisionEndCallback != nullptr) {
            other->CollisionEndCallback(this);
        }
        if(CollisionEndCallback != nullptr) {
            CollisionEndCallback(other);
        }
    }
    else {
        // do the same for triggers
        if(colliderType == ColliderType::Trigger && other->TriggerOverlapEndCallback != nullptr) {
            other->TriggerOverlapEndCallback(this);
        }
        if(other->colliderType == ColliderType::Trigger && TriggerOverlapEndCallback != nullptr) {
            TriggerOverlapEndCallback(other);
        }
    }
}

/* void Collider::ResolveTriggerOverlapAgainstPlayer(Player& player) {
    if (colliderType == ColliderType::Solid) return;

    bool overlapping = rect.getGlobalBounds().intersects(
        player.collisionRect.getGlobalBounds());

    if (overlapping && !_overlappingPlayer) {
        _overlappingPlayer = true;
        rect.setFillColor(sf::Color::Green);
        for (auto c : OverlapBeginCallbacks) {
            c(&player);
        }
    } else if (!overlapping && _overlappingPlayer) {
        _overlappingPlayer = false;
        rect.setFillColor(sf::Color::Yellow);
        for (auto c : OverlapBeginCallbacks) {
            c(&player);
        }
    }
} */

/* void Collider::ResolveCollisionAgainstPlayer(Player& player) {
    if (colliderType == ColliderType::Trigger) return;

    // Test collision for player
    if (rect.getGlobalBounds().intersects(
            player.collisionRect.getGlobalBounds())) {
        if (HasCollisionDirectionEnabled(Bottom) && player.velocity.y < 0 &&
            player.prevPosition.y > (rect.getPosition().y + rect.getSize().y +
                                     player.collisionRect.getSize().y)) {
            // We hit the bottom
            player.velocity.y = 0;
            player.setPosition(player.getPosition().x,
                               rect.getPosition().y + rect.getSize().y +
                                   player.collisionRect.getSize().y);
        } else if (HasCollisionDirectionEnabled(Top) && player.velocity.y > 0 &&
                   player.prevPosition.y <= (rect.getPosition().y)) {
            // We hit the top
            player.grounded = true;
            player.velocity.y = 0;
            player.setPosition(player.getPosition().x, rect.getPosition().y);
        } else if (HasCollisionDirectionEnabled(Left) &&
                   player.velocity.x > 0 &&
                   (player.prevPosition.x + player.collisionRect.getSize().x /
                                                2) <= rect.getPosition().x) {
            // We hit the left side
            player.velocity.x = 0;
            player.setPosition(
                rect.getPosition().x - player.collisionRect.getSize().x / 2,
                player.getPosition().y);
        } else if (HasCollisionDirectionEnabled(Right) &&
                   player.velocity.x < 0 &&
                   (player.prevPosition.x -
                    player.collisionRect.getSize().x / 2) >=
                       rect.getPosition().x + rect.getSize().x) {
            // We hit the right side
            player.velocity.x = 0;
            player.setPosition(rect.getPosition().x + rect.getSize().x +
                                   player.collisionRect.getSize().x / 2,
                               player.getPosition().y);
        }
    }
}
 */
std::vector<Collider> LoadCollidersFromConfig() {
    std::ifstream config("config/collision.json");
    std::stringstream configBuffer;
    configBuffer << config.rdbuf();

    try {
        nlohmann::json json;
        json = nlohmann::json::parse(configBuffer.str());
        auto j = json["colliders"];
        std::vector<Collider> returnValue;
        // iterate the array
        for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
            std::cout << *it << '\n';
            nlohmann::json value = it.value();
            Collider c;
            float collisionSizeX = value["width"].get<float>();
            float collisionSizeY = value["height"].get<float>();
            c.rect.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
            float posX = value["x"].get<float>();
            float posY = value["y"].get<float>();
            c.rect.setPosition(posX, posY);
            c.colliderType = (ColliderType)value["type"].get<int>();
            c.collisionDirection =
                (CollisionDirection)value["direction"].get<int>();
            returnValue.push_back(c);
        }
        return returnValue;

    } catch (const std::exception& e) {
        std::cout << "Error opening config file." << std::endl;
        std::cerr << e.what() << std::endl;
        return std::vector<Collider>();
    }
}