#pragma once
#include <vector>
#include "Collider.h"

struct Physics {
    static std::unordered_map<Collider*, bool> colliders;
    static void RegisterCollider(Collider* col);
    static void DeregisterCollider(Collider* col);
    static void CheckForCollisionsAndTriggerOverlaps();
};