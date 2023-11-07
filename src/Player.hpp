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

namespace PlayerGlobal {
    inline int radiation_received = 0; 
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
        health -= damaged;

        if(health <= 0) {
            PlayerState::currentState = PlayerState::dead;
            PlayerState::currentState->enter();

            health = 0;
        }
    }

    void damage(const int damaged) {
        health -= damaged;
        SFX::damage.play();

        PlayerState::currentState = PlayerState::damageState;
        PlayerState::currentState->enter();

        if(health <= 0) {
            PlayerState::currentState = PlayerState::dead;
            PlayerState::currentState->enter();

            health = 0;
        }
    }

private:
    int health = 100;

    bool grounded = false;

    sf::Sprite spr;
    sf::Texture tx;

    sf::Sprite sprite;


    IntPhysicsComponent physComp;
};