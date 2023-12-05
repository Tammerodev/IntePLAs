#pragma once
#include "Mob.hpp"
#include "FrogDamageState.hpp"
#include "FrogDeadState.hpp"
#include "FrogIdleState.hpp"
#include "FrogJumpState.hpp"

#include "PhysicsComponent.hpp"

#include <vector>


class Frog : public Mob {
    public:
        void load();
        void update(const float dt);
        void collisionCheck(VoxelManager&);
        void render(sf::RenderTarget&);

        void invoke(const MobInvoke&);
        bool remove();

        PhysicsComponent& getPhysicsComponent();

    protected:
        unsigned int maxHealth = 0;
        PhysicsComponent physicsComponent;

        sf::Sprite sprite;
        sf::Texture texture;   

        MobInvoke mobInvoke;

        float distanceWhenInvoked = 50.f;

        bool grounded = false;    
};