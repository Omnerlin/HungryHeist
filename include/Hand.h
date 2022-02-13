#pragma once
#include <SFML/Graphics.hpp>

enum HandSpawnDirection { FromLeft = 0, FromRight = 1, FromTop = 2, FromBottom = 3 };

struct Hand : sf::Sprite {
    static sf::Color SkinColors[];
    enum HandState { Waiting, Attacking, Retreating };
    void Attack(HandSpawnDirection from, sf::View view, float speed);
    void SetOpen(bool opened);
    bool IsOpen();
    void Update(float deltaTime);
    bool done {true};

   private:
    void SetHandState(HandState state);
    HandState _currentState;
    float _speed;
    sf::Vector2f _targetPosition;
    sf::Vector2f _homePosition;
    float _returnDelay = 0.5f;
    float _timeSinceStateChange;
    HandSpawnDirection _attackDirection;
    bool _open;
};
