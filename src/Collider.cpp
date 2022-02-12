#include <Collider.h>
#include <Player.h>

bool Collider::HasCollisionDirectionEnabled(CollisionDirection direction) {
    return (direction | collisionDirection) == collisionDirection;
}

bool Collider::ResolveCollisionAgainstPlayer(Player& player) {
    if(colliderType == ColliderType::Trigger) return false;

        // Test collision for player
    if(rect.getGlobalBounds().intersects(player.collisionRect.getGlobalBounds())) {
        if(HasCollisionDirectionEnabled(Bottom) && player.velocity.y < 0 && player.prevPosition.y > (rect.getPosition().y + rect.getSize().y + player.collisionRect.getSize().y)) {
            // We hit the bottom
            player.velocity.y = 0;
            player.setPosition(player.getPosition().x, rect.getPosition().y + rect.getSize().y + player.collisionRect.getSize().y);
        }
        else if(HasCollisionDirectionEnabled(Top) &&  player.velocity.y > 0 && player.prevPosition.y <= (rect.getPosition().y)) {
            // We hit the top
            player.grounded = true;
            player.velocity.y = 0;
            player.setPosition(player.getPosition().x, rect.getPosition().y);
        }
        else if(HasCollisionDirectionEnabled(Left) && player.velocity.x > 0 && (player.prevPosition.x + player.collisionRect.getSize().x / 2) <= rect.getPosition().x) {
            // We hit the left side
            player.velocity.x = 0;
            player.setPosition(rect.getPosition().x - player.collisionRect.getSize().x / 2, player.getPosition().y);
        }
        else if(HasCollisionDirectionEnabled(Right) && player.velocity.x < 0 && (player.prevPosition.x - player.collisionRect.getSize().x / 2) >= rect.getPosition().x + rect.getSize().x) {
            // We hit the right side
            player.velocity.x = 0;
            player.setPosition(rect.getPosition().x + rect.getSize().x + player.collisionRect.getSize().x / 2, player.getPosition().y);
        }            
    }

    return true;
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
            Collider c;
            float collisionSizeX = it.value()["width"].get<float>();
            float collisionSizeY = it.value()["height"].get<float>();
            c.rect.setSize(sf::Vector2f(collisionSizeX, collisionSizeY));
            float posX = it.value()["x"].get<float>();
            float posY = it.value()["y"].get<float>();
            c.rect.setPosition(posX, posY);
            c.colliderType = (ColliderType)it.value()["type"].get<int>();
            returnValue.push_back(c);
        }
        return returnValue;

    } catch (const std::exception& e) {
        std::cout << "Error opening config file." << std::endl;
        std::cerr << e.what() << std::endl;
        return std::vector<Collider>();
    }
}