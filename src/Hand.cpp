#include <Hand.h>
#include <Utils.h>

sf::Color Hand::SkinColors[]{
	sf::Color(255, 219, 172),
	sf::Color(241, 194, 125),
	sf::Color(224, 172, 105),
	sf::Color(198, 134, 66),
	sf::Color(141, 85, 36),
};

siv::PerlinNoise::seed_type Hand::seed = 123456u;
siv::PerlinNoise Hand::Noise = siv::PerlinNoise(Hand::seed);

void Hand::Attack(HandSpawnDirection from, sf::View view, float speed, float offset) {
	done = false;
	_speed = speed;
	_attackDirection = from;
	sf::Vector2f viewHalfSize = view.getSize() / 2.f;
	switch (from) {
	case HandSpawnDirection::FromLeft:
		transform.SetLocalRotation(90);
		_homePosition = sf::Vector2f(-viewHalfSize.x, -offset);
		_targetPosition = sf::Vector2f(viewHalfSize.x, -offset);
		exclamationSprite.setPosition(_homePosition.x, _homePosition.y - 16);
		break;
	case HandSpawnDirection::FromTop:
		transform.SetLocalRotation(180);
		_homePosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
		_targetPosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
		exclamationSprite.setPosition(_homePosition.x - 8, _homePosition.y);
		break;
	case HandSpawnDirection::FromRight:
		transform.SetLocalRotation(270);
		_homePosition = sf::Vector2f(viewHalfSize.x, -offset);
		_targetPosition = sf::Vector2f(-viewHalfSize.x, -offset);
		exclamationSprite.setPosition(_homePosition.x - 16, _homePosition.y - 16);
		break;
	case HandSpawnDirection::FromBottom:
		transform.SetLocalRotation(0);
		_homePosition = sf::Vector2f(-viewHalfSize.x + offset, 0);
		_targetPosition = sf::Vector2f(-viewHalfSize.x + offset, -viewHalfSize.y * 2);
		exclamationSprite.setPosition(_homePosition.x - 8, _homePosition.y - 32);
		break;
	}
	transform.SetWorldPosition(_homePosition);
	SetHandState(HandState::Warning);
}

void Hand::SetHandState(HandState state) {
	_timeSinceStateChange = 0;
	oldPosition = transform.GetWorldPosition();
	switch (state) {
	case HandState::Warning:
		sonarSound.play();
		grabTrigger.enabled = false;
		break;
	case HandState::Attacking:
		grabTrigger.enabled = true;
		//grabTrigger.colliderType = ColliderType::Trigger;
		break;

	case HandState::Waiting:
		grabTrigger.enabled = false;
		if (!capturedPlayer)
		{
			handSprite.setColor(sf::Color(125, 125, 125));
		}
		//grabTrigger.colliderType = ColliderType::Solid;
		break;

	case HandState::Retreating:
		grabTrigger.enabled = false;
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
	handSprite.setTextureRect(
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
	{
		float intensity = 1.5f;
		float noiseX = Noise.normalizedOctave1D(_timeSinceStateChange * 50, 1);
		float noiseY = Noise.normalizedOctave1D(_timeSinceStateChange * 80, 1);
		transform.SetWorldPosition(oldPosition.x + noiseX * intensity, oldPosition.y + noiseY * intensity);
		if (_timeSinceStateChange >= _returnDelay)
		{
			SetHandState(HandState::Retreating);
		}
	}
	break;

	case HandState::Warning:	// Warning that it is going to attack
		if (_timeSinceStateChange >= warningDuration)
			SetHandState(HandState::Attacking);
		break;

	case HandState::Attacking:	// Move towards target
		posX = Lerp(_homePosition.x, _targetPosition.x, _timeSinceStateChange / _speed);
		posY = Lerp(_homePosition.y, _targetPosition.y, _timeSinceStateChange / _speed);

		if (_timeSinceStateChange >= _speed) {
			transform.SetWorldPosition(_targetPosition);
			handFinishSound.play();
			SetHandState(HandState::Waiting);
		}
		else {
			transform.SetWorldPosition(posX, posY);
		}
		break;

	case HandState::Retreating: // Move back to home
		if (capturedPlayer) return;
		float colorA = Lerp(255, 0, _timeSinceStateChange / _speed);
		if (_timeSinceStateChange >= _speed) {
			handSprite.setColor(sf::Color::Transparent);
			transform.SetWorldPosition(_homePosition);
			done = true;
			if (HandFinishCallback != nullptr) {
				HandFinishCallback();
			}
			SetHandState(HandState::Waiting);
		}
		else {
			handSprite.setColor(sf::Color(handSprite.getColor().r, handSprite.getColor().g, handSprite.getColor().b, colorA));
		}
		break;
	}
}

void Hand::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	if(!done && _currentState != HandState::Warning)
	{
		target.draw(handSprite, states);
	}
	if (_currentState == HandState::Warning) {
		target.draw(exclamationSprite, states);
	}
}
