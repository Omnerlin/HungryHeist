#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <Collider.h>
#include "EntityTransform.h"
#include "PerlinNoise.hpp"

enum class HandSpawnDirection { FromLeft = 0, FromRight = 1, FromTop = 2, FromBottom = 3 };

struct HandSpawnPosition {
    HandSpawnDirection direction;
    float offset;
};

struct Hand : public sf::Drawable {

    // Members
    static sf::Color SkinColors[];
    static siv::PerlinNoise::seed_type seed;
    static siv::PerlinNoise Noise;
    enum HandState { Waiting, Warning, Attacking, Retreating };
    EntityTransform transform;
    EntityTransform grabTriggerTranform;
    sf::Sprite handSprite;
    sf::Sprite exclamationSprite;
    Collider grabTrigger;
    std::function<void()> HandFinishCallback;
    float warningDuration = 0.5f;
    bool done {true};

    // Methods
    void Attack(HandSpawnDirection from, sf::View view, float speed, float offset);
    void SetOpen(bool opened);
    bool IsOpen();
    void SetHandState(HandState state);
    HandState GetHandState();
    void Update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Inherited via Drawable

   private:
    HandState _currentState;
    float _speed;
    sf::Vector2f _targetPosition;
    sf::Vector2f _homePosition;
    float _returnDelay {0.5f};
    float _timeSinceStateChange;
    HandSpawnDirection _attackDirection;
    bool _open;

};
