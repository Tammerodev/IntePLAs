#pragma once
#include "Rigidbody.hpp"
#include <SFML/Graphics.hpp>

class RectangleRigidbody : public RigidBody {
    public:
        void loadFromRectangle(const sf::FloatRect& rect) {
            PhysicsComponent point0 = PhysicsComponent(sf::Vector2f(rect.top, rect.left));
            PhysicsComponent point1 = PhysicsComponent(sf::Vector2f(rect.top + rect.width, rect.left));
            PhysicsComponent point2 = PhysicsComponent(sf::Vector2f(rect.top, rect.left + rect.height));
            PhysicsComponent point3 = PhysicsComponent(sf::Vector2f(rect.top + rect.width, rect.left + rect.height));

            points.emplace_back(point0);
            points.emplace_back(point1);
            points.emplace_back(point2);
            points.emplace_back(point3);
        }
    private:
};