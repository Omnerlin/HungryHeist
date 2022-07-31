#pragma once
#include <vector>

enum EaseType;

struct Tween {
	float duration{0.5f};
	bool free{true};
	EaseType EaseType;
	void Update(float deltaTime)
	{
		
	}

};

struct Tweener {
	static Tween CreateTween();

private:
	std::vector<Tween> tweens;
};

enum EaseType
{
	InSine,
	OutSine,
	InOutSine,

	InCubic,
	OutCubic,
	InOutCubic,

	InQuint,
	OutQuint,
	InOutQuint,

	InCirc,
	OutCirc,
	InOutCirc,

	InElasic,
	OutElastic,
	InOutElastic,

	InQuad,
	OutQuad,
	InOutQuad,

	InQuart,
	OutQuart,
	InOutQuart,

	InExpo,
	OutExpo,
	InOutExpo,

	InBack,
	OutBack,
	InOutBack,

	InBounce,
	OutBounce,
	InOutBounce
};
