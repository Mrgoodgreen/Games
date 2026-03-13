#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Collider.h"

namespace ArkanoidGame
{
    enum class BounceAxis
    {
        None,
        Horizontal,
        Vertical
    };

    class Collision
    {
    public:
        static bool Intersects(const sf::FloatRect& bounds, const Collider& collider)
        {
            return bounds.intersects(collider.GetBounds());
        }

        static BounceAxis GetBounceAxis(const sf::Vector2f& center, float radius, const sf::FloatRect& targetBounds)
        {
            const float targetCenterX = targetBounds.left + targetBounds.width / 2.f;
            const float targetCenterY = targetBounds.top + targetBounds.height / 2.f;

            const float dx = std::abs(center.x - targetCenterX) - (targetBounds.width / 2.f + radius);
            const float dy = std::abs(center.y - targetCenterY) - (targetBounds.height / 2.f + radius);

            return (dx > dy) ? BounceAxis::Horizontal : BounceAxis::Vertical;
        }

        static void ReflectVelocity(sf::Vector2f& velocity, BounceAxis axis)
        {
            if (axis == BounceAxis::Horizontal)
            {
                velocity.x = -velocity.x;
            }
            else if (axis == BounceAxis::Vertical)
            {
                velocity.y = -velocity.y;
            }
        }
    };
}
