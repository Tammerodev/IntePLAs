#include "MobManager.hpp"

void MobManager::update(const float dt, Player& player) {
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        auto fish = std::make_shared<Fish>();
        fish->getPhysicsComponent().transform_position = player.getPhysicsComponent().transform_position;
        fish->load();

        mobs.push_back(fish);
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
