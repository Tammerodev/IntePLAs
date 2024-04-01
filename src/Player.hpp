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
#include "PlayerGlobal.hpp"
#include "PlayerGlobal.hpp"
#include "SwimState.hpp"
#include "debug_globals.hpp"
#include "PlayerTemparatureManager.hpp"

#include "PlayerHealthManager.hpp"

class Player {
public:

    int load();
    void update(ChunkIndexer& world, float);
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

    PlayerHealthManager& getHealthManager() {
        return healthManager;
    }

private:
    PlayerHealthManager healthManager;
    bool grounded = false;

    sf::Sprite spr;
    sf::Texture tx;

    sf::Sprite sprite;
    IntPhysicsComponent physComp;

    PlayerTemparatureManager playerTempManager;
};