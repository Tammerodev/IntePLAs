#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "PhysicsComponent.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

struct Constraint {
    uint8_t indexA;
    uint8_t indexB;
    float restLength;
};

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

        void setVelocity(const sf::Vector2f& vel = {0.f, 0.f}) {
            for(auto &p : points) {
                p.velocity = vel;
            }
        }
    protected:
        PhysicsComponent cog;
        std::vector<PhysicsComponent> points;
        std::vector<Constraint> constraints;
};