#pragma once
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <functional>
#include <unordered_set>

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
    std::unordered_set<Collider*> isCollidingOrOverlappingWith;
    sf::RectangleShape rect;
    std::function<void(Collider*)> CollisionBeginCallback, CollisionStayCallback, CollisionEndCallback, 
        TriggerOverlapBeginCallback, TriggerOverlapStayCallback, TriggerOverlapEndCallback;
    bool HasCollisionDirectionEnabled(CollisionDirection direction);
    void InvokeInitialCallbackAgainstCollider(Collider* collider);
    void InvokeStayCallbackAgainstCollider(Collider* collider);
    void InvokeEndCallbackAgainstCollider(Collider* collider);

    private:
    bool _overlappingPlayer;
};

std::vector<Collider> LoadCollidersFromConfig();