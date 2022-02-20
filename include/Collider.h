#pragma once
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <functional>

struct Player;

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
    std::vector<std::function<void(Player*)>> OverlapBeginCallbacks;
    std::vector<std::function<void(Player*)>> OverlapEndCallbacks;
    bool HasCollisionDirectionEnabled(CollisionDirection direction);
    void ResolveCollisionAgainstPlayer(Player& player);
    void ResolveTriggerOverlapAgainstPlayer(Player& player);

    private:
    bool _overlappingPlayer;
};

std::vector<Collider> LoadCollidersFromConfig();