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
            exclamationSprite.setPosition(_homePosition.x, _homePosition.y - 16);
            break;
        case HandSpawnDirection::FromTop:
            setRotation(180);
            _homePosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
            _targetPosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
            exclamationSprite.setPosition(_homePosition.x - 8, _homePosition.y);
            break;
        case HandSpawnDirection::FromRight:
            setRotation(270);
            _homePosition = sf::Vector2f(viewHalfSize.x, -offset);
            _targetPosition = sf::Vector2f(-viewHalfSize.x, -offset);
            exclamationSprite.setPosition(_homePosition.x - 16, _homePosition.y - 16);
            break;
        case HandSpawnDirection::FromBottom:
            setRotation(0);
            _homePosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
            _targetPosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
            exclamationSprite.setPosition(_homePosition.x - 8, _homePosition.y - 32);
            break;
    }
    setPosition(_homePosition);
    SetHandState(HandState::Warning);
}

void Hand::SetHandState(HandState state) {
    _timeSinceStateChange = 0;
    switch(state) {
        case HandState::Attacking:
            //grabTrigger.colliderType = ColliderType::Trigger;
        break;

        case HandState::Waiting:
            setColor(sf::Color(125,125,125));
            //grabTrigger.colliderType = ColliderType::Solid;
        break;

        case HandState::Retreating:
            //grabTrigger.colliderType = ColliderType::Trigger;
        break;

        default:
        break;
    }
    _currentState = state;
    SetOpen(state == HandState::Attacking);
}

bool Hand::IsOpen() { return _open; }

void Hand::SetOpen(bool open) {
    setTextureRect(
        sf::IntRect(sf::Vector2i(open ? 64 : 0, 0), sf::Vector2i(64, 72)));
    _open = open;
}

Hand::HandState Hand::GetHandState() {
    return _currentState;
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
            {
                SetHandState(HandState::Retreating);
            }
            break;

        // Warning that it is going to attack
        case HandState::Warning:
            if (_timeSinceStateChange >= warningDuration)
                SetHandState(HandState::Attacking);
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
            float colorA = lerp(255, 0, _timeSinceStateChange/_speed);
            //posX = lerp(_targetPosition.x, _homePosition.x,  _timeSinceStateChange / _speed);
            //posY = lerp(_targetPosition.y, _homePosition.y, _timeSinceStateChange / _speed);

            if(_timeSinceStateChange >= _speed) {
                setColor(sf::Color::Transparent);
                setPosition(_homePosition);
                done = true;
                if(HandFinishCallback != nullptr) {
                    HandFinishCallback();
                }
                SetHandState(HandState::Waiting);
            } else {
                setColor(sf::Color(getColor().r, getColor().g, getColor().b, colorA));
            }
            break;
    }
    grabTrigger.rect.setPosition(getPosition());
    grabTrigger.rect.setRotation(getRotation());
}