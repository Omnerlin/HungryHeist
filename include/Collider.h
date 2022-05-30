#pragma once
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <functional>
#include <unordered_set>
#include <DrawnEntity.h>

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

struct Collider : DrawnEntity<sf::RectangleShape> {
    ColliderType colliderType = ColliderType::Solid;
    CollisionDirection collisionDirection = CollisionDirection::All;
    std::unordered_set<Collider*> isCollidingOrOverlappingWith;
    std::vector<std::function<void(Collider*)>> CollisionBeginCallbacks, CollisionStayCallback, CollisionEndCallback, 
        TriggerOverlapBeginCallback, TriggerOverlapStayCallback, TriggerOverlapEndCallback;
    bool HasCollisionDirectionEnabled(CollisionDirection direction);
    void InvokeInitialCallbackAgainstCollider(Collider* collider);
    void InvokeStayCallbackAgainstCollider(Collider* collider);
    void InvokeEndCallbackAgainstCollider(Collider* collider);
    void InvokeCallbacks(const std::vector<std::function<void(Collider*)>>& callbacks, Collider* arg);
	bool enabled { true };
};

std::vector<Collider> LoadCollidersFromConfig();