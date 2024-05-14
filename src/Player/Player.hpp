#pragma once

#include "../Controls.hpp"
#include <SFML/Graphics.hpp>
#include "PlayerState.hpp"
#include "IdleState.hpp"
#include "WalkState.hpp"
#include "JumpState.hpp"
#include "DeadState.hpp"
#include "CreativeState.hpp"
#include "DamageState.hpp"
#include "../PhysicsComponent.hpp"
#include <iostream>
#include "../SoundFX.hpp"
#include "RespawnState.hpp"
#include "PlayerGlobal.hpp"
#include "PlayerGlobal.hpp"
#include "SwimState.hpp"
#include "../debug_globals.hpp"
#include "PlayerTemparatureManager.hpp"

#include "PlayerHealthManager.hpp"
#include "PlayerHungerManager.hpp"

class Player {
public:

    int load();
    void update(ChunkIndexer& world, float, Camera &cam);
    void draw(sf::RenderTarget&);

    IntPhysicsComponent &getPhysicsComponent() {
        return physComp;
    }

    const sf::FloatRect getHitbox() const {
        return spr.getGlobalBounds();
    }

    const sf::Vector2f getHeadPosition() {
        return physComp.transform_position + sf::Vector2f(8, 3);
    }

    void ground() {
        if(prevVelY > 6.f) {
            healthManager.damageHit(prevVelY);
        }

        physComp.velocity.y = 0;
        physComp.velocity.x /= 1.5;
        
        grounded = true;
    }

    PlayerHealthManager& getHealthManager() {
        return healthManager;
    }

private:
    float prevVelY = 0.f;

    PlayerHealthManager healthManager;
    PlayerHungerManager hungerManager;

    bool grounded = false;

    sf::Sprite spr;
    sf::Texture tx;

    sf::Sprite sprite;
    IntPhysicsComponent physComp;

    PlayerTemparatureManager playerTempManager;
};