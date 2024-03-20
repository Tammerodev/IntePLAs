#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "PhysicsComponent.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class RigidBody {
    public:
        void calculatePoints(ChunkIndexer& world) {
            for(auto &point : points) {
                // Apply gravity
                point.update();
                point.velocity.y += 0.05;

                const int collision = world.getPixelCollision(point.transform_position).first;
                if(collision) { 
                    point.velocity.y--;
                    point.velocity.y = 0.f;
                }

                loginf(point.transform_position.x, "<x :::: y>", point.transform_position.y);

            }  
        }

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