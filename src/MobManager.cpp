#include "MobManager.hpp"

void MobManager::update(const float dt) {
    auto &particles = mobs;

    for (auto it = mobs.begin(); it != mobs.end();) {
        auto& mob = *it;

        mob->update(dt);

        if(mob->remove()) {
            it = mobs.erase(it);
        } else {
            ++it;
        }
    }   
}

void MobManager::invokeMobs(Player &player) {
    for(const auto mob : mobs) {
        const float distance = math::distance(player.getPhysicsComponent().transform_position, mob->getPhysicsComponent().transform_position);
        MobInvoke mobInvoke;
        mobInvoke.distanceToPlayer = distance;

        mobInvoke.playerSubVector = math::subVector(player.getPhysicsComponent().transform_position, mob->getPhysicsComponent().transform_position);

        mob->invoke(mobInvoke);
    }
}
