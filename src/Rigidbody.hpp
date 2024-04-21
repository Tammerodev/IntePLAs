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
                circle.setRadius(1.0);
                

                target.draw(circle);
            }
            

            sf::CircleShape circle;
            circle.setPosition(cog.transform_position);
            circle.setFillColor(sf::Color::Blue);
            circle.setRadius(3.0);
            

            target.draw(circle);
        }
    protected:
        PhysicsComponent cog;
        std::vector<PhysicsComponent> points;
};