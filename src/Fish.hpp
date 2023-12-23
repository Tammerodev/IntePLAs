#pragma once
#include "Mob.hpp"
#include "FrogDamageState.hpp"
#include "FrogDeadState.hpp"
#include "FrogIdleState.hpp"
#include "FrogJumpState.hpp"

#include "PhysicsComponent.hpp"
#include "CollisionManager.hpp"

#include <vector>


class Fish : public Mob {
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

        int scaredTimer = 0;
        const int scaredTime = 500;

        const float speed_division = 0.1;

        bool do_remove = false;

        FastNoiseLite fsl;

        sf::Clock timer;
        int time_offset = 0;
};