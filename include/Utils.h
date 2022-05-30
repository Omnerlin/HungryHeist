#pragma once
#include "SFML/Graphics.hpp"

/// <summary>
/// Linear interpolation between two floating point numbers by t
/// </summary>
/// <returns></returns>
inline float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

/// <summary>
/// Multiply each scalar component of the supplied vector by the corrosponding components of vector "by"
/// </summary>
/// <returns>A new scaled vector</returns>
inline const sf::Vector2f& ScaleVector(const sf::Vector2f& vector, const sf::Vector2f& by) {
    return { vector.x * by.x, vector.y * by.y };
}