#pragma once
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>

class Player;

enum ColliderType {
    Solid,
    Trigger
};

// What direction a collider will detect collision from. 
// For example, no collision from bottom would mean a "jump-through" platform.
enum CollisionDirection {
    Left = 1,
    Right = 2,
    Top = 4,
    Bottom = 8,
    All = Left|Right|Top|Bottom
};

struct Collider {
    ColliderType colliderType = ColliderType::Solid;
    CollisionDirection collisionDirection = CollisionDirection::All;
    sf::RectangleShape rect;
    bool HasCollisionDirectionEnabled(CollisionDirection direction);
    bool ResolveCollisionAgainstPlayer(Player& player);
};

std::vector<Collider> LoadCollidersFromConfig();