#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "PhysicsComponent.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class RigidBody {
    public:
        void debugRender(sf::RenderTarget& target) {
            for(const auto &point : points) {
                sf::CircleShape circle;
                circle.setPosition(point.transform_position);
                circle.setFillColor(sf::Color::Red);
                circle.setRadius(2.0);
                

                target.draw(circle);
            }
        }
    protected:
        std::vector<PhysicsComponent> points;
};