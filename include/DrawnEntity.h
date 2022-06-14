#pragma once
#include "EntityTransform.h"
template <class T> 
struct DrawnEntity : sf::Drawable
{
	DrawnEntity();
	DrawnEntity(const DrawnEntity& other);
	T drawable;
	EntityTransform transform;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Inherited via Drawable
	~DrawnEntity() override = default;
};


template <class T>
DrawnEntity<T>::DrawnEntity()
{
	transform.SetAttachedTransformable(&drawable);
}

template <class T>
void DrawnEntity<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(drawable, states);
}

template <class T>
DrawnEntity<T>::DrawnEntity(const DrawnEntity& other) {
	transform = other.transform;
	drawable = other.drawable;
	transform.SetAttachedTransformable(&drawable);
}