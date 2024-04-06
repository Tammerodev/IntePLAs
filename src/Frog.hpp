#pragma once
#include "Mob.hpp"
#include "FrogDamageState.hpp"
#include "FrogDeadState.hpp"
#include "FrogIdleState.hpp"
#include "FrogJumpState.hpp"

#include "PhysicsComponent.hpp"
#include "CollisionManager.hpp"

#include <vector>


class Frog : public Mob {
    public:
        void load();
        void update(const float dt);
        void collisionCheck(VoxelManager&);
        void render(sf::RenderTarget&);

        void invoke(const MobInvoke&);
        bool remove(VoxelManager&);

        PhysicsComponent& getPhysicsComponent();

    protected:
        std::array<FrogState*, 10> states {
            nullptr,
            new FrogDamageState(),
            new FrogJumpState(),
            new FrogDeadState(),
            new FrogIdleState()
        };

        FrogStateType::Type currentState = FrogStateType::FrogIdle;

        unsigned int maxHealth = 0;
        PhysicsComponent physicsComponent;
        
        MobInvoke mobInvoke;

        DefaultBehaviour default_behaviour;

        float distanceWhenInvoked = 50.f;

        bool grounded = false;    

        int scaredTimer = 0;
        const int scaredTime = 500;

        const float frogSpeed = 1.0;
};