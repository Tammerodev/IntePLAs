#pragma once
#include <SFML/Main.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>
#include "common.hpp"
#include "math.hpp"

namespace PhysicsGlobals {
    inline float gravity = 0.1f;
}

struct IntPhysicsComponent {
    sf::Vector2i transform_origin = sf::Vector2i(0, 0);
    sf::Vector2f transform_position = sf::Vector2f(0, 0);

    void update(float dt = 1) {

        velocity.y += (PhysicsGlobals::gravity * dt);

        
        transform_position += sf::Vector2f(velocity) * dt;
        transform_position.y += velocity.y;
    

    }

    void setVelocity(const sf::Vector2f& v) {
        velocity = v;
    }

    void addToVelocity(const sf::Vector2f& v) {
        velocity += v;
    }

    sf::Vector2f velocity {0, 0};

    float weight = 0.5f;
    float friction = 1.0f;
};

struct PhysicsComponent {
    sf::Vector2f transform_origin = sf::Vector2f(0.f, 0.f);
    sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f transform_position = sf::Vector2f(0.f, 0.f);
    float transform_rotation = 0.f;

    PhysicsComponent() {

    }

    PhysicsComponent(const sf::Vector2f& position) {
        transform_position = position;
    }

    sf::Vector2f rotate_point(float cx, float cy, float angle, sf::Vector2f p){

        return sf::Vector2f(cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
                    sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy);
    }

    void applyGravity(float dt = 0.0) {
        if(has_gravity)
            velocity.y += (PhysicsGlobals::gravity * dt);
    }

    void applyTransforms() {             
        transform_position += velocity;

        float rotation = std::atan2(velocity.y, velocity.x) * (180.0 / math::PI);

        transform_rotation = rotation;
    }
    
    void update(float dt = 0.0) {
        if(has_gravity)
            velocity.y += (PhysicsGlobals::gravity * dt);
            
        transform_position += velocity;

        float rotation = std::atan2(velocity.y, velocity.x) * (180.0 / math::PI);

        transform_rotation = rotation;
    }

    void disable_gravity() {
        has_gravity = false;
    }

private:
    bool has_gravity = true;

};