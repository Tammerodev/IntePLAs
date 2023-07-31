#pragma once
#include <SFML/Main.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>


struct PhysicsComponent {
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f transform_position = sf::Vector2f(0.f, 0.f);
    float transform_rotation = 0.f;

    float gravity = 0.03f;

    void update() {
        velocity.y += gravity;
        transform_position += velocity;

        float rotation = std::atan2(velocity.y, velocity.x) * (180.0 / M_PI);

        transform_rotation = rotation;
    }

};