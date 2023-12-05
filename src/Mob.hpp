#pragma once
#include "Entity.hpp"
#include "VoxelManager.hpp"

struct MobInvoke {
    float damage = 0.f;
    float distanceToPlayer = 0.f;

    sf::Vector2f playerSubVector = sf::Vector2f(0.f, 0.f);
};

class Mob : public Entity {
    public:
        virtual void load() = 0;
        virtual void update(const float dt) = 0;
        virtual void collisionCheck(VoxelManager&) = 0;
        virtual void render(sf::RenderTarget&) = 0;

        virtual void invoke(const MobInvoke&) = 0;
        virtual bool remove() = 0;

        virtual PhysicsComponent& getPhysicsComponent() = 0;

    protected:
        unsigned int maxHealth = 0;
};