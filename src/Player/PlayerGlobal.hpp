#pragma once
#include "PlayerState.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <algorithm>

namespace PlayerGlobal {
    inline float radiation_received = 0; 
    inline int still_radioation = 0;

    inline int max_health = 16;
    inline int health = max_health;

    inline int max_hunger = 16;
    inline int hunger = max_hunger;

    inline float temparature = 0;
    inline float bodyTemparature = 0;

    inline float getTemparature() {
        return (std::clamp(temparature / 100.f, -250.f, 250.f) / 1);
    }

    inline void updateBodyTemparature() {
        const float tempDiff = temparature - bodyTemparature;
        const float tempChangeSpeed = 0.01f;

        if(abs(tempDiff) < 1.0f) return;
        bodyTemparature += tempDiff * tempChangeSpeed;
    }

    inline void respawn() {
        PlayerGlobal::health = PlayerGlobal::max_health;
        PlayerGlobal::radiation_received = 0;
        PlayerGlobal::still_radioation = 0;

        PlayerState::currentState = PlayerState::respawnState;
        PlayerState::currentState->enter();
    }

    inline sf::Vector2u characterSize = sf::Vector2u(18, 28);
    inline sf::Vector2u characterHitBoxSize = sf::Vector2u(18, 28);
    inline int character_animation_frames = 3;

    inline sf::Vector2f spawn_point = sf::Vector2f(0.f, 0.f);

}
