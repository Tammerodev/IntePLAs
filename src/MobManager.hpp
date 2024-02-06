#pragma once
#include "Frog.hpp"
#include "Fish.hpp"
#include <list>
#include "MobInfoBar.hpp"
#include "ProceduralGeneration.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class MobManager {
    public:
        void load() {

            for(const auto &mob : mobs) {
                mob->load();
            }
        }

        void addMob(const std::shared_ptr<Mob> mob) {
            mob->load();
            mobs.push_back(mob);
        }

        void spawnMobs(ProcGenerate& proceduralGeneration, ChunkIndexer& world) {
            for(int i = 0; i < worldSize::world_sx - 1; i++) {
                Biome &biome = proceduralGeneration.getBiomeAtPosition(i, world);
                BiomeInfo* biomeInfo = nullptr;
                biomeInfo = (BiomeInfo*)biome.getInfo();

                if(math::randProp(biomeInfo->mobInfo.amount)) {
                    const float h = proceduralGeneration.getHeightOnMap(i);

                    int randomIndex = 0;
                    if(biomeInfo->mobInfo.spawns.empty()) continue;

                    randomIndex = math::randIntInRange(0, biomeInfo->mobInfo.spawns.size() - 1);

                    const MobType randomMobType = biomeInfo->mobInfo.spawns.at(randomIndex).first;

                    spawnMobOfType(randomMobType, sf::Vector2f(i, h - 30));
                }
            }

        }

        void spawnMobOfType(MobType type, sf::Vector2f position) {
            std::shared_ptr<Mob> mob;

            switch (type)
            {
            case MobType::Fish:
                mob = std::make_shared<Fish>();
                break;

            case MobType::Frog:
                mob = std::make_shared<Frog>();
                break;
            
            default:
                return;
            }

            mob->getPhysicsComponent().transform_position = position;
            mob->load();
            mobs.emplace_back(mob);
        }

        void checkCollisions(VoxelManager& voxelManager) {
            for(const auto &mob : mobs) {
                mob->collisionCheck(voxelManager);
            }
        }

        const unsigned int getMobCount() {
            return mobs.size();
        }

        void update(const float dt, Player& player, VoxelManager&); 

        void render(sf::RenderTarget &target) {
            for(const auto &mob : mobs) {
                mob->render(target);
            }
        }

        void invokeMobs(Player& player, std::vector<ExplosionInfo>& points);
        
    private:
        std::list<std::shared_ptr<Mob>> mobs;
};
