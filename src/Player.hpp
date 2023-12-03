#pragma once

#include "Controls.hpp"
#include <SFML/Graphics.hpp>
#include "PlayerState.hpp"
#include "IdleState.hpp"
#include "WalkState.hpp"
#include "JumpState.hpp"
#include "DeadState.hpp"
#include "CreativeState.hpp"
#include "DamageState.hpp"
#include "PhysicsComponent.hpp"
#include <iostream>
#include "Collider.hpp"
#include "SoundFX.hpp"
#include "RespawnState.hpp"

namespace PlayerGlobal {
    inline int radiation_received = 0; 
    inline int still_radioation = 0;

    inline int max_health = 500;
    inline int health = max_health;

    inline void respawn() {
        PlayerGlobal::health = PlayerGlobal::max_health;
        PlayerGlobal::radiation_received = 0;
        PlayerGlobal::still_radioation = 0;

        PlayerState::currentState = PlayerState::respawnState;
        PlayerState::currentState->enter();
    }
}

class Player {
public:

    int load();
    void update(float);
    void draw(sf::RenderTarget&);

    IntPhysicsComponent &getPhysicsComponent() {
        return physComp;
    }

    void ground() {
        physComp.velocity.y = 0;
        physComp.velocity_buffer = 0;
        physComp.velocity.x /= 2;
        
        grounded = true;
    }

    void damage_radiation(const int damaged) {
        PlayerGlobal::health -= damaged;

        if(PlayerGlobal::health <= 0) {
            PlayerState::currentState = PlayerState::dead;
            PlayerState::currentState->enter();

            PlayerGlobal::health = 0;
        }
    }

    void damage(const int damaged) {
        PlayerGlobal::health -= damaged;
        SFX::damage.play();

        PlayerState::currentState = PlayerState::damageState;
        PlayerState::currentState->enter();

        if(PlayerGlobal::health <= 0) {
            PlayerState::currentState = PlayerState::dead;
            PlayerState::currentState->enter();

            PlayerGlobal::health = 0;
        }
    }

private:

    bool grounded = false;

    sf::Sprite spr;
    sf::Texture tx;

    sf::Sprite sprite;


    IntPhysicsComponent physComp;
};