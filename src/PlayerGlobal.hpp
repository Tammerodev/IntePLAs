#pragma once
#include "PlayerState.hpp"
#include <SFML/Graphics/Vertex.hpp>

namespace PlayerGlobal {
    inline int radiation_received = 0; 
    inline int still_radioation = 0;

    inline int max_health = 16;
    inline int health = max_health;

    inline float temparature = 0;

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
