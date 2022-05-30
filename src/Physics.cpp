#include "Physics.h"

#include <algorithm>
#include <iostream>
#include <map>

std::unordered_map<Collider*, bool> Physics::colliders =
    std::unordered_map<Collider*, bool>();

void Physics::RegisterCollider(Collider* col) {
    if (colliders.find(col) == colliders.end()) {
        colliders.emplace(col, false);
    } else {
        std::cout << "Tried to register collider " << col
                  << " but it is already registered" << std::endl;
    }
}

void Physics::DeregisterCollider(Collider* col) {
    // Find collider and then erase
    auto colItr = colliders.find(col);
    if (colItr != colliders.end()) {
        colliders.erase(colItr);
    } else {
        std::cout << "Tried to deregister nonexistent collider " << col
                  << std::endl;
    }
}

void Physics::CheckForCollisionsAndTriggerOverlaps() {
    // Reset collider "Checked" state
    for (auto& col : colliders) {
        col.second = false;
    }
    // Check all colliders against other colliders
    for (auto& col : colliders) {
        for (auto& colTemp : colliders) {
            if (col.first == colTemp.first || !col.first->enabled || !colTemp.first->enabled) {
                continue;  // Don't compare a collider against itself or against
                           // colliders that have already run through the list.
            } else {
                // Check for intersection
                if (col.first->drawable.getGlobalBounds().intersects(
                        colTemp.first->drawable.getGlobalBounds())) {
                    if (col.first->isCollidingOrOverlappingWith.find(
                            colTemp.first) ==
                        col.first->isCollidingOrOverlappingWith.end()) {
                        // We're colliding or overlapping for the first time
                        col.first->isCollidingOrOverlappingWith.emplace(
                            colTemp.first);
                        colTemp.first->isCollidingOrOverlappingWith.emplace(
                            col.first);
                        col.first->InvokeInitialCallbackAgainstCollider(colTemp.first);
                        col.first->InvokeStayCallbackAgainstCollider(colTemp.first);
                    } else {
                        // We've collided before and are still colliding
                        col.first->InvokeStayCallbackAgainstCollider(colTemp.first);
                    }
                } else {
                    // If we were colliding but are now separated
                    auto colItr = col.first->isCollidingOrOverlappingWith.find(colTemp.first);
                    if (colItr !=
                        col.first->isCollidingOrOverlappingWith.end()) {
                        // remove the things
                        col.first->isCollidingOrOverlappingWith.erase(colItr);
                        colTemp.first->isCollidingOrOverlappingWith.erase(col.first);
                        col.first->InvokeEndCallbackAgainstCollider(colTemp.first);
                    }
                }
            }
            col.second = true;
        }
    }
}