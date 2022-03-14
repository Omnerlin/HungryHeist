#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>

struct GameEntity {
	GameEntity* parent{ nullptr };

	// Position
	void SetWorldPosition(const sf::Vector2f& position) {
		if (parent != nullptr) {
			_localTransformable.setPosition(parent->_worldTransformable.getInverseTransform().transformPoint(position));
		}
		else {
			_localTransformable.setPosition(position);
		}
		UpdateTransforms();
	}

	void SetWorldPosition(float x, float y) {
		SetWorldPosition(sf::Vector2f(x, y));
	}


	void SetLocalPosition(const sf::Vector2f& position) {
		_localTransformable.setPosition(position);
		UpdateTransforms();
	}

	void SetLocalPosition(float x, float y) {
		SetLocalPosition(sf::Vector2f(x, y));
	}

	// Rotation
	void SetLocalRotation(float rotation) {
		_localTransformable.setRotation(rotation);
		UpdateTransforms();
	}

	void SetWorldRotation(float rotation) {
		if (parent != nullptr) {
			rotation -= parent->_worldTransformable.getRotation();
		}
		_localTransformable.setRotation(rotation);
		UpdateTransforms();
	}

	// Scale
	void SetLocalScale(const sf::Vector2f scale) {
		_localTransformable.setScale(scale);
		UpdateTransforms();
	}

	void SetLocalScale(float x, float y) {
		SetLocalScale(sf::Vector2f(x, y));
	}

	// Parent/child management
	void SetParent(GameEntity* newParent) {
		GameEntity* prevParent = parent;
		if (prevParent != nullptr) {
			prevParent->_children.erase(this);
		}
		parent = newParent;
		if (parent != nullptr) {
			parent->_children.insert(this);
		}

		float prevRot = _worldTransformable.getRotation();
		SetWorldPosition(_worldTransformable.getPosition());
		SetWorldRotation(prevRot);
	}

	// Update this transform and its children
	void UpdateTransforms() {
		if (parent != nullptr) {
			_worldTransformable.setRotation(parent->_worldTransformable.getRotation() + _localTransformable.getRotation());
			_worldTransformable.setPosition(parent->_worldTransformable.getTransform().transformPoint(_localTransformable.getPosition()));
			_worldTransformable.setScale(sf::Vector2f(parent->_worldTransformable.getScale().x * _localTransformable.getScale().x,
				parent->_worldTransformable.getScale().y * _localTransformable.getScale().y));
		}
		else {
			_worldTransformable.setPosition(_localTransformable.getPosition());
			_worldTransformable.setRotation(_localTransformable.getRotation());
			_worldTransformable.setScale(_localTransformable.getScale());
		}
		for (GameEntity* e : _children) {
			e->UpdateTransforms();
		}
	}

	sf::Transformable _worldTransformable;
	sf::Transformable _localTransformable;
	std::unordered_set<GameEntity*> _children;

private:
};