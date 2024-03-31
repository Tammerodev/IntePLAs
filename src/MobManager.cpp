#include "MobManager.hpp"

void MobManager::update(const float dt, Player& player, VoxelManager& vx) {
    for (auto it = mobs.begin(); it != mobs.end();) {
        auto& mob = *it;

        mob->update(dt);

        if(mob->remove(vx)) {
            it = mobs.erase(it);
        } else {
            ++it;
        }
    }   

    debug_globals::mob_count = getMobCount();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        auto fish = std::make_shared<Frog>();
        fish->getPhysicsComponent().transform_position = player.getPhysicsComponent().transform_position;
        fish->load();

        mobs.push_back(fish);
    }
}

void MobManager::invokeMobs(Player &player, std::vector<ExplosionInfo>& points) {
    for(const auto &mob : mobs) {
        const float distance_to_player = math::distance(player.getPhysicsComponent().transform_position, mob->getPhysicsComponent().transform_position);
        MobInvoke mobInvoke;
        mobInvoke.distanceToPlayer = distance_to_player;

        for(const auto point : points) {
            const sf::Vector2f mob_position = mob->getPhysicsComponent().transform_position;
            const float dist = math::distance(mob_position, point.position);

            if(dist < point.strength) {
                float damage = point.strength - dist;

                if(damage > mobInvoke.damage) {
                    mobInvoke.damage = damage;
                } 
            }   
        }
        mobInvoke.playerSubVector = math::subVector(player.getPhysicsComponent().transform_position, mob->getPhysicsComponent().transform_position);

        mob->invoke(mobInvoke);
    }
}
