#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>

struct EntityTransform {

	// Position
	void SetWorldPosition(const sf::Vector2f& position);
	void SetWorldPosition(float x, float y);

	void SetLocalPosition(const sf::Vector2f& position);
	void SetLocalPosition(float x, float y);

	const sf::Vector2f& GetWorldPosition();
	const sf::Vector2f& GetLocalPosition();

	void MoveWorld(const sf::Vector2f& moveAmount);
	void MoveWorld(float x, float y);
	void MoveLocal(const sf::Vector2f& moveAmount);
	void MoveLocal(float x, float y);

	// Rotation
	void SetLocalRotation(float rotation);
	void SetWorldRotation(float rotation);

	float GetLocalRotation();
	float GetWorldRotation();

	void WorldRotateBy(float degrees);
	void LocateRotateBy(float degrees);

	// Scale
	void SetLocalScale(const sf::Vector2f& scale);
	void SetLocalScale(float x, float y);
	void SetWorldScale(const sf::Vector2f& scale);
	const sf::Vector2f& GetLossyScale();
	const sf::Vector2f& GetLocalScale();
	const sf::Vector2f& GetWorldScale();

	// Parent/child management
	void SetParent(EntityTransform* newParent);
	EntityTransform* const GetParent();
	bool HasChild(EntityTransform* entity);
	void SetAttachedTransformable(sf::Transformable* transform);
	void RemoveAttachedTransform();

	// Update this transform and its children
	void UpdateTransforms();

protected:
	EntityTransform* _parent{ nullptr };
	sf::Transformable* _attachedTransform{ nullptr };
	std::unordered_set<EntityTransform*> _children;

private:
	sf::Transformable _worldTransformable;
	sf::Transformable _localTransformable;
};