#pragma once
#include <SFML/Main.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>

struct IntPhysicsComponent {
    sf::Vector2i transform_origin = sf::Vector2i(0, 0);
    sf::Vector2i velocity = sf::Vector2i(0, 0);
    sf::Vector2i transform_position = sf::Vector2i(0, 0);

    float gravity = 0.001;

    void update(int dt = 1) {
            
        velocity_buffer += (gravity * dt);
        //velocity.y = velocity_buffer;

        transform_position += velocity;
        transform_position.y += floor(velocity_buffer);


    }

    void setVelocity(const sf::Vector2f& v) {
        velocity_buffer = v.y;
        velocity.x = (int)v.x;
    }

    int timer = 0;
    float velocity_buffer = 0.0;

    float weight = 2.0;
    float friction = 1.0;
};

struct PhysicsComponent {
    sf::Vector2f transform_origin = sf::Vector2f(0.f, 0.f);
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f transform_position = sf::Vector2f(0.f, 0.f);
    float transform_rotation = 0.f;

    float gravity = 0.001f;

    sf::Vector2f rotate_point(float cx, float cy, float angle, sf::Vector2f p){

        return sf::Vector2f(cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
                    sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy);
    }
    
    void update(float dt = 0.0) {
        velocity.y += (gravity * dt);
        transform_position += velocity;

        float rotation = std::atan2(velocity.y, velocity.x) * (180.0 / M_PI);

        transform_rotation = rotation;
    }

};