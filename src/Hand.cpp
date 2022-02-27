#include <Hand.h>
#include <Utils.h>

sf::Color Hand::SkinColors[] {
    sf::Color(255, 219, 172),
    sf::Color(241, 194, 125),
    sf::Color(224, 172, 105),
    sf::Color(198, 134, 66),
    sf::Color(141, 85, 36),
};

void Hand::Attack(HandSpawnDirection from, sf::View view, float speed, float offset) {
    done = false;
    _speed = speed;
    _attackDirection = from;
    sf::Vector2f viewHalfSize = view.getSize() / 2.f;
    switch (from) {
        case HandSpawnDirection::FromLeft:
            setRotation(90);
            _homePosition = sf::Vector2f(-viewHalfSize.x, -offset);
            _targetPosition = sf::Vector2f(viewHalfSize.x, -offset);
            break;
        case HandSpawnDirection::FromTop:
            setRotation(180);
            _homePosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
            _targetPosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
            break;
        case HandSpawnDirection::FromRight:
            setRotation(270);
            _homePosition = sf::Vector2f(viewHalfSize.x, -offset);
            _targetPosition = sf::Vector2f(-viewHalfSize.x, -offset);
            break;
        case HandSpawnDirection::FromBottom:
            setRotation(0);
            _homePosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
            _targetPosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
            break;
    }
    setPosition(_homePosition);
    SetHandState(HandState::Attacking);
}

void Hand::SetHandState(HandState state) {
    _timeSinceStateChange = 0;
    _currentState = state;
    SetOpen(state == HandState::Attacking);
}

bool Hand::IsOpen() { return _open; }

void Hand::SetOpen(bool open) {
    setTextureRect(
        sf::IntRect(sf::Vector2i(open ? 64 : 0, 0), sf::Vector2i(64, 281)));
    _open = open;
}

void Hand::Update(float deltaTime) {
    if (done) return;
    _timeSinceStateChange += deltaTime;
    float posX;
    float posY;
    switch (_currentState) {
        // Pausing before retreat
        case HandState::Waiting:
            if (_timeSinceStateChange >= _returnDelay)
                SetHandState(HandState::Retreating);
            break;

        // Move towards target
        case HandState::Attacking: 
            posX = lerp(_homePosition.x, _targetPosition.x, _timeSinceStateChange / _speed);
            posY = lerp(_homePosition.y, _targetPosition.y, _timeSinceStateChange / _speed);

            if(_timeSinceStateChange >= _speed) {
                setPosition(_targetPosition);
                SetHandState(HandState::Waiting);
            } else {
                setPosition(posX, posY);
            }
            break;

        // Move back to home
        case HandState::Retreating: 
            posX = lerp(_targetPosition.x, _homePosition.x,  _timeSinceStateChange / _speed);
            posY = lerp(_targetPosition.y, _homePosition.y, _timeSinceStateChange / _speed);

            if(_timeSinceStateChange >= _speed) {
                setPosition(_homePosition);
                done = true;
                SetHandState(HandState::Waiting);
            } else {
                setPosition(posX, posY);
            }
            break;
    }
    grabTrigger.rect.setPosition(getPosition());
    grabTrigger.rect.setRotation(getRotation());
}