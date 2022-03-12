#include <Collider.h>
#include <Player.h>

bool Collider::HasCollisionDirectionEnabled(CollisionDirection direction) {
    return (direction | collisionDirection) == collisionDirection;
}

void Collider::InvokeInitialCallbackAgainstCollider(Collider* other) {
    if (colliderType == ColliderType::Solid &&
        other->colliderType == ColliderType::Solid) {
        if (other->CollisionBeginCallback != nullptr) {
            other->CollisionBeginCallback(this);
        }
        if (CollisionBeginCallback != nullptr) {
            CollisionBeginCallback(other);
        }
    } else {
        // do the same for triggers
        if (colliderType == ColliderType::Trigger &&
            other->TriggerOverlapBeginCallback != nullptr) {
            other->TriggerOverlapBeginCallback(this);
        }
        if (other->colliderType == ColliderType::Trigger &&
            TriggerOverlapBeginCallback != nullptr) {
            TriggerOverlapBeginCallback(other);
        }
    }
}

void Collider::InvokeStayCallbackAgainstCollider(Collider* other) {
    if (colliderType == ColliderType::Solid &&
        other->colliderType == ColliderType::Solid) {
        if (other->CollisionStayCallback != nullptr) {
            other->CollisionStayCallback(this);
        }
        if (CollisionStayCallback != nullptr) {
            CollisionStayCallback(other);
        }
    } else {
        // do the same for triggers
        if (colliderType == ColliderType::Trigger &&
            other->TriggerOverlapStayCallback != nullptr) {
            other->TriggerOverlapStayCallback(this);
        }
        if (other->colliderType == ColliderType::Trigger &&
            TriggerOverlapStayCallback != nullptr) {
            TriggerOverlapStayCallback(other);
        }
    }
}

void Collider::InvokeEndCallbackAgainstCollider(Collider* other) {
    if (colliderType == ColliderType::Solid &&
        other->colliderType == ColliderType::Solid) {
        if (other->CollisionEndCallback != nullptr) {
            other->CollisionEndCallback(this);
        }
        if (CollisionEndCallback != nullptr) {
            CollisionEndCallback(other);
        }
    } else {
        // do the same for triggers
        if (colliderType == ColliderType::Trigger &&
            other->TriggerOverlapEndCallback != nullptr) {
            other->TriggerOverlapEndCallback(this);
        }
        if (other->colliderType == ColliderType::Trigger &&
            TriggerOverlapEndCallback != nullptr) {
            TriggerOverlapEndCallback(other);
        }
    }
}

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