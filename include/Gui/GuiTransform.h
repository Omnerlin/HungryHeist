#pragma once
#include "SFML/Graphics.hpp"
#include "EntityTransform.h"

struct GuiTransform : public EntityTransform {

public:
	GuiTransform();
	void MarkDirty();
	void RebuildLayout(bool force = false);
	void UpdateTransforms() override;

	// Rectangle methods
	void SetRectSize(const sf::Vector2f& size, bool markDirty = true);
	void SetRectSize(float width, float height, bool markDirty = true);
	const sf::Vector2f& GetRectSize();
	const sf::Vector2f& GetSizeDelta();
	const sf::RectangleShape& GetRectDrawable() const;
	void SetColor(sf::Color color);
	const sf::Color GetDebugColor() const;

	// Anchoring, Pivot
	void SetAnchoredPosition(const sf::Vector2f& anchoredPosition);
	void SetAnchoredPosition(float x, float y);

	void SetAnchorMin(const sf::Vector2f& anchorMin);
	void SetAnchorMin(float x, float y);
	const sf::Vector2f& GetAnchorMin() const;

	void SetAnchorMax(const sf::Vector2f& anchorMax);
	void SetAnchorMax(float x, float y);
	const sf::Vector2f& GetAnchorMax() const;
	
	const sf::Vector2f& GetAnchoredPosition() const;

	// Pivot/Origin
	void SetOrigin(const sf::Vector2f& origin) override;
	void SetPivot(const sf::Vector2f& pivot);
	const sf::Vector2f& GetPivot();

	// Utils
	void CopyAttributesFrom(GuiTransform& other);

protected:
	sf::Vector2f _anchorMin {0,0};
	sf::Vector2f _anchorMax {0,0};
	sf::Vector2f _anchoredPosition{0,0};
	sf::Vector2f _pivot {0,0};

private:
	sf::Vector2f sizeDelta;
	sf::RectangleShape rect; // This isn't meant to be used for visuals - only for debug drawing and keeping track of GUI size changes.
	sf::Color debug_color;
	bool _needsRebuild {false};

};

