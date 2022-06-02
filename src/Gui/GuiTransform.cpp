#include "Gui/GuiTransform.h"
#include "Utils.h"
#include <iostream>


GuiTransform::GuiTransform()
{
	rect.setFillColor(sf::Color::Transparent);
}

void GuiTransform::MarkDirty()
{
	_needsRebuild = true;
	for (EntityTransform* e : _children) {
		GuiTransform* child = dynamic_cast<GuiTransform*>(e);
		if (child != nullptr) {
			child->MarkDirty();
		}
	}
}

void GuiTransform::RebuildLayout(bool force)
{
	if ((!_needsRebuild && !force) || (_parent == nullptr)) return;

	_needsRebuild = false;
	GuiTransform* parent = dynamic_cast<GuiTransform*>(_parent);
	if (parent == nullptr) {
		// This means our parent isn't null, but it isn't a GUI transform either... just act like we're a root.
		std::cout << "WARNING: trying to rebuild with a non-gui parent.";
	}

	// Now that all that checking is done, we can setl out what we had planned to do (update our rect).

	// Update our rect size 
	sf::Vector2f anchorDifference = _anchorMax - _anchorMin;
	sf::Vector2f oldSize = rect.getSize();
	sf::Vector2f targetSizeDelta = ScaleVector(parent->sizeDelta, anchorDifference);
	rect.setSize(GetRectSize() + targetSizeDelta);

	// Update our rect position based on that of our parent
	// In order to tell if we should move, we need know from what position the parent's rectangle size has changed.
	// We'll do this by taking their origin into account versus their rectangle size. 
	sf::Vector2f anchorMidpoint = (_anchorMin + _anchorMax) / 2.f;
	sf::Vector2f prevPosition = ScaleVector(parent->GetRectSize() - parent->sizeDelta, anchorMidpoint);
	sf::Vector2f newPosition = ScaleVector(parent->GetRectSize(), anchorMidpoint);
	sf::Vector2f posDelta = (newPosition - prevPosition);

	// Actually, we might need to take into account the following:
	// 1. Parent pivot.x distance to anchor min x for left expansion
	// 2. Parent pivot.x distance to anchor max x for right expansion
	// 3. Parent pivot.y distance to anchor min y for up expansion
	// 4. Parent pivot.y distance to anchor max y for down expansion
	// Depending on final requirements and our pivot, we'll either expand or move.
	// For example, if we need to expand left by 50 pixels and right by 50 pixels, but our pivot is at 0,0, we'll expand right by 100 pixels and move left by 50.
	//float amountExpandLeft = parent->sizeDelta.x * parent->GetPivot().x;
	//float amountExpandRight = parent->sizeDelta.x * (1 - parent->GetPivot().x);
	//float amountExpandUp = parent->sizeDelta.y * parent->GetPivot().y;
	//float amountExpandDown = parent->sizeDelta.y * (1 - parent->GetPivot().y);

	float amountToMoveX = targetSizeDelta.x * (0.5 - GetPivot().x);
	float amountToMoveY = targetSizeDelta.y * (0.5 - GetPivot().y);


	// Get the distance between the paren't pivot and the center of the anchor point to get percentage movement.
	sf::Vector2f parentPivot = parent->GetPivot();
	//sf::Vector2f movePercentage(abs(anchorMidpoint.x - parentPivot.x), abs(anchorMidpoint.y - parentPivot.y));
	//sf::Vector2f movePercentage(abs(anchorMidpoint.x - parent->GetPivot().x) - anchorMidpoint.x, abs(anchorMidpoint.y - parent->GetPivot().y) - anchorMidpoint.y);
	 // sf::Vector2f movePercentage(1 - abs(_anchorMax.x - parentPivot.x), abs(anchorMidpoint.y - parentPivot.y));
	sizeDelta = rect.getSize() - oldSize;
	//SetLocalPosition(GetLocalPosition() + posDelta);
	SetLocalPosition(GetLocalPosition() + (sf::Vector2f(-amountToMoveX, -amountToMoveY)) + posDelta);
	SetPivot(_pivot);

	//if (rect.getFillColor().r == 255) {
	//	std::cout << std::to_string(GetWorldPosition().x) << " " << std::to_string(GetWorldPosition().y) << std::endl;
	//}

	//if (rect.getFillColor().g == 255) {
	//	std::cout << "Target size delta: " << std::to_string(targetSizeDelta.x)  << " " << std::to_string(targetSizeDelta.y) << std::endl;
	//}
}

void GuiTransform::UpdateTransforms()
{
	RebuildLayout();
	EntityTransform::UpdateTransforms();
	rect.setOrigin(GetWorldTransform().getOrigin());
	rect.setPosition(GetWorldTransform().getPosition());
	rect.setRotation(GetWorldTransform().getRotation());
	rect.setScale(GetWorldTransform().getScale());
}

void GuiTransform::SetRectSize(const sf::Vector2f& size, bool markDirty)
{
	sf::Vector2f oldSize = rect.getSize();
	rect.setSize(size);
	SetPivot(_pivot);
	if (markDirty) {
		sizeDelta = size - oldSize;
		MarkDirty();
	}
}

void GuiTransform::SetRectSize(float width, float height, bool markDirty)
{
	SetRectSize({ width, height }, markDirty);
}

const sf::Vector2f& GuiTransform::GetRectSize()
{
	return rect.getSize();
}

const sf::Vector2f& GuiTransform::GetSizeDelta()
{
	return sizeDelta;
}

const sf::RectangleShape& GuiTransform::GetRectDrawable() const
{
	return rect;
}

void GuiTransform::SetColor(sf::Color color)
{
	rect.setFillColor(color);
	//rect.setOutlineThickness(-3);
	//rect.setOutlineColor(sf::Color::Black);
	debug_color = color;
}

const sf::Color GuiTransform::GetDebugColor() const
{
	return debug_color;
}

void GuiTransform::SetAnchoredPosition(const sf::Vector2f& anchoredPosition)
{
	this->_anchoredPosition = anchoredPosition;
	MarkDirty();
}

void GuiTransform::SetAnchoredPosition(float x, float y)
{
	SetAnchoredPosition({ x, y });
}

void GuiTransform::SetAnchorMin(const sf::Vector2f& anchorMin)
{
	_anchorMin = anchorMin;
}

void GuiTransform::SetAnchorMin(float x, float y)
{
	SetAnchorMin(sf::Vector2f( x, y ));
}

const sf::Vector2f& GuiTransform::GetAnchorMin() const
{
	return _anchorMin;
}

void GuiTransform::SetAnchorMax(const sf::Vector2f& anchorMax)
{
	_anchorMax = anchorMax;
}

void GuiTransform::SetAnchorMax(float x, float y)
{
	SetAnchorMax(sf::Vector2f( x, y ));
}

const sf::Vector2f& GuiTransform::GetAnchorMax() const
{
	return _anchorMax;
}

const sf::Vector2f& GuiTransform::GetAnchoredPosition() const
{
	return _anchoredPosition;
}

void GuiTransform::SetOrigin(const sf::Vector2f& origin)
{
	EntityTransform::SetOrigin(origin);

	sf::Vector2f size = rect.getSize();
	_pivot = sf::Vector2f(size.x > 0 ? origin.x / rect.getSize().x : origin.x, size.y > 0 ? origin.y / rect.getSize().y : origin.y);
}

void GuiTransform::SetPivot(const sf::Vector2f& pivot)
{
	_pivot = pivot;
	SetOrigin(ScaleVector(rect.getSize(), pivot));
}

const sf::Vector2f& GuiTransform::GetPivot()
{
	return _pivot;
}

void GuiTransform::CopyAttributesFrom(GuiTransform& other)
{
	SetRectSize(other.GetRectSize());
	SetPivot(other.GetPivot());
	SetAnchorMin(other.GetAnchorMin());
	SetAnchorMax(other.GetAnchorMax());
}
