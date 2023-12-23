#pragma once
#include "Frog.hpp"
#include "Fish.hpp"
#include <list>

class MobManager {
    public:
        void load() {
            mobs.push_back(std::make_shared<Frog>());

            for(const auto mob : mobs) {
                mob->load();
            }
        }

        void addMob(const std::shared_ptr<Mob> mob) {
            mob->load();
            mobs.push_back(mob);
        }

        void checkCollisions(VoxelManager& voxelManager) {
            for(const auto mob : mobs) {
                mob->collisionCheck(voxelManager);
            }
        }

        const unsigned int getMobCount() {
            return mobs.size();
        }

        void update(const float dt, Player& player); 

        void render(sf::RenderTarget &target) {
            for(const auto mob : mobs) {
                mob->render(target);
            }
        }

        void invokeMobs(Player& player);
        
    private:
        std::list<std::shared_ptr<Mob>> mobs;
};
