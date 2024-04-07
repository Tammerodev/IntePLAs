#pragma once
#include "Mob/Frog/Frog.hpp"
#include "Mob/Fish/Fish.hpp"
#include <list>
#include "MobInfoBar.hpp"
#include "ProceduralGeneration.hpp"
#include "ChunkIndexerVoxelContainer.hpp"
#include "MobIDGenerator.hpp"

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

        bool getCollisionsInPoint(const sf::Vector2f& pos) {
            for(const auto &mob : mobs) {
                if(mob->getHitbox().contains(pos))
                    return true;
            }

            return false;
        }

        bool damageMobsIfCollision(const sf::Vector2f& pos, int damage) {
            for(const auto &mob : mobs) {
                if(mob->getHitbox().contains(pos)) {
                    MobInvoke invoke {};
                    invoke.damage = damage;
                    mob->invoke(invoke);
                }
            }

            return false;
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
            
            mob->setID(MobIDGenerator::getNextID());
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

                if(debug_globals::inDebugDisplayState) {
                    sf::RectangleShape rect;
                    rect.setSize(sf::Vector2f(mob->getHitbox().width, mob->getHitbox().height));
                    rect.setPosition(mob->getHitbox().left, mob->getHitbox().top);

                    rect.setOutlineThickness(1.0f);
                    rect.setOutlineColor(sf::Color::White);

                    target.draw(rect);
                }
            }
        }

        void invokeMobs(Player& player, std::vector<ExplosionInfo>& points);
        
    private:
        std::list<std::shared_ptr<Mob>> mobs;
};
