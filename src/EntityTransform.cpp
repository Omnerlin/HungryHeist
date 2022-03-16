#include "EntityTransform.h"

void EntityTransform::SetWorldPosition(const sf::Vector2f& position) {
	if (_parent != nullptr) {
		_localTransformable.setPosition(_parent->_worldTransformable.getInverseTransform().transformPoint(position));
	}
	else {
		_localTransformable.setPosition(position);
	}
	UpdateTransforms();
}

void EntityTransform::SetWorldPosition(float x, float y) {
	SetWorldPosition(sf::Vector2f(x, y));
}

void EntityTransform::SetLocalPosition(const sf::Vector2f& position) {
	_localTransformable.setPosition(position);
	UpdateTransforms();
}

void EntityTransform::SetLocalPosition(float x, float y) {
	SetLocalPosition(sf::Vector2f(x, y));
}

const sf::Vector2f& EntityTransform::GetWorldPosition() {
	return _worldTransformable.getPosition();
}

const sf::Vector2f& EntityTransform::GetLocalPosition() {
	return _localTransformable.getPosition();
}

void EntityTransform::MoveWorld(const sf::Vector2f& moveAmount) {
	SetWorldPosition(GetWorldPosition() + moveAmount);
}

void EntityTransform::MoveWorld(float x, float y) {
	SetWorldPosition(GetWorldPosition() + sf::Vector2f(x, y));
}

void EntityTransform::MoveLocal(const sf::Vector2f& moveAmount) {
	SetLocalPosition(GetLocalPosition() + moveAmount);
}

void EntityTransform::MoveLocal(float x, float y) {
	SetLocalPosition(GetLocalPosition() + sf::Vector2f(x, y));
}

void EntityTransform::SetLocalRotation(float rotation) {
	_localTransformable.setRotation(rotation);
	UpdateTransforms();
}

void EntityTransform::SetWorldRotation(float rotation) {
	if (_parent != nullptr) {
		rotation -= _parent->_worldTransformable.getRotation();
	}
	_localTransformable.setRotation(rotation);
	UpdateTransforms();
}

float EntityTransform::GetLocalRotation() {
	return _localTransformable.getRotation();
}

float EntityTransform::GetWorldRotation() {
	return _worldTransformable.getRotation();
}

void EntityTransform::WorldRotateBy(float degrees) {
	SetWorldRotation(GetWorldRotation() + degrees);
}

void EntityTransform::LocateRotateBy(float degrees) {
	SetLocalRotation(GetLocalRotation() + degrees);
}

void EntityTransform::SetLocalScale(const sf::Vector2f& scale) {
	_localTransformable.setScale(scale);
	UpdateTransforms();
}

void EntityTransform::SetLocalScale(float x, float y) {
	SetLocalScale(sf::Vector2f(x, y));
}

void EntityTransform::SetWorldScale(const sf::Vector2f& scale) {
	if (_parent != nullptr) {
		_localTransformable.setScale({ _parent->GetWorldScale().x == 0 ? 0 : (scale.x / _parent->GetWorldScale().x),
			_parent->GetWorldScale().y == 0 ? 0 : (scale.y / _parent->GetWorldScale().y) });
	}
	else {
		_localTransformable.setScale(scale);
	}
	UpdateTransforms();
}

const sf::Vector2f& EntityTransform::GetLossyScale() {
	if (_parent != nullptr) {
		sf::Vector2f result = GetLossyScale();
		sf::Vector2f localScale = GetLocalScale();

		return { result.x * localScale.x, result.y * localScale.y };
	}
	else {
		return GetLocalScale();
	}
}

const sf::Vector2f& EntityTransform::GetLocalScale() {
	return _localTransformable.getScale();
}

const sf::Vector2f& EntityTransform::GetWorldScale() {
	return _worldTransformable.getScale();
}

void EntityTransform::SetParent(EntityTransform* newParent) {
	// Safety check against self-assignment, redundant assignment, and child assignment
	if (newParent == this || newParent == _parent || HasChild(newParent)) {
		return;
	}

	EntityTransform* prevParent = _parent;
	if (prevParent != nullptr) {
		prevParent->_children.erase(this);
	}
	_parent = newParent;
	if (_parent != nullptr) {
		_parent->_children.insert(this);
	}

	float prevRot = _worldTransformable.getRotation();
	sf::Vector2f prevScale = _worldTransformable.getScale();
	SetWorldPosition(_worldTransformable.getPosition());
	SetWorldRotation(prevRot);
	SetWorldScale(prevScale);
}

EntityTransform* const EntityTransform::GetParent() {
	return _parent;
}

bool EntityTransform::HasChild(EntityTransform* entity) {
	if (entity == nullptr) return false;
	EntityTransform* _tempParent = entity->_parent;
	while (_tempParent != nullptr) {
		if (_tempParent == this) {
			return true;
		}
		else {
			_tempParent = _tempParent->_parent;
		}
	}
	return false;
}

void EntityTransform::SetAttachedTransformable(sf::Transformable* transform) {
	_attachedTransform = transform;
}

void EntityTransform::RemoveAttachedTransform() {
	_attachedTransform = nullptr;
}

void EntityTransform::UpdateTransforms() {
	if (_parent != nullptr) {
		_worldTransformable.setRotation(_parent->_worldTransformable.getRotation() + _localTransformable.getRotation());
		_worldTransformable.setPosition(_parent->_worldTransformable.getTransform().transformPoint(_localTransformable.getPosition()));
		_worldTransformable.setScale(sf::Vector2f(_parent->_worldTransformable.getScale().x * _localTransformable.getScale().x,
			_parent->_worldTransformable.getScale().y * _localTransformable.getScale().y));
	}
	else {
		_worldTransformable.setPosition(_localTransformable.getPosition());
		_worldTransformable.setRotation(_localTransformable.getRotation());
		_worldTransformable.setScale(_localTransformable.getScale());
	}
	if (_attachedTransform != nullptr) {
		_attachedTransform->setPosition(_worldTransformable.getPosition());
		_attachedTransform->setRotation(_worldTransformable.getRotation());
		_attachedTransform->setScale(_worldTransformable.getScale());
	}

	for (EntityTransform* e : _children) {
		e->UpdateTransforms();
	}
}
