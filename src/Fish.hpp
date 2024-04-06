#pragma once
#include "Mob.hpp"
#include "FishDamageState.hpp"
#include "FishDeadState.hpp"
#include "FishIdleState.hpp"
#include "FishSwimState.hpp"

#include "PhysicsComponent.hpp"
#include "CollisionManager.hpp"

#include <vector>
#include "common.hpp"

class Fish : public Mob {
    public:
        void load();
        void update(const float dt);
        void collisionCheck(VoxelManager&);
        void render(sf::RenderTarget&);

        void invoke(const MobInvoke&);
        bool remove(VoxelManager&);

        PhysicsComponent& getPhysicsComponent();

    protected:
        std::array<FishState*, 10> states {
            nullptr,
            new FishDamageState(),
            new FishSwimState(),
            new FishDeadState(),
            new FishIdleState()
        };

        FishStateType::Type currentState = FishStateType::FishIdle;


        unsigned int maxHealth = 0;
        PhysicsComponent physicsComponent;
        DefaultBehaviour default_behaviour;
        
        MobInvoke mobInvoke;

        float distanceWhenInvoked = 50.f;

        bool grounded = false;    

        int scaredTimer = 0;
        const int scaredTime = 500;

        const float speed_division = 0.5;

        FastNoiseLite fsl;

        sf::Clock timer;
        int time_offset = 0;
};