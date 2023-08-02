#pragma once
#include <SFML/Main.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>


struct PhysicsComponent {
    sf::Vector2f transform_origin = sf::Vector2f(0.f, 0.f);
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f transform_position = sf::Vector2f(0.f, 0.f);
    float transform_rotation = 0.f;

    float gravity = 0.01f;

    sf::Vector2f rotate_point(float cx, float cy, float angle, sf::Vector2f p){

        return sf::Vector2f(cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
                    sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy);
    }
    
    void update() {
        velocity.y += gravity;
        transform_position += velocity;

        float rotation = std::atan2(velocity.y, velocity.x) * (180.0 / M_PI);

        transform_rotation = rotation;
    }

};