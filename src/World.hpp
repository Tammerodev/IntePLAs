#pragma once 
#include <list>
#include <algorithm>
#include <iostream>
#include "Sound/GameSoundManager.hpp"
#include "VoxelManager.hpp"
#include "Player/Player.hpp"
#include "VoxelGroup.hpp"
#include "WeatherManager.hpp"
#include "CollisionManager.hpp"
#include "MobManager.hpp"
#include "VoxelGroupPropeties/VoxelGroupPropetiesHandler.hpp"

#include "AddVoxelGroups.hpp"

namespace fs = std::filesystem;

class World {
public:
    bool init(const std::string path, sf::Vector2f* player_pos) {
        bool res = true;
        
        load_state::setState(load_state::Loading_map);
        if(!main_world.load(path, player_pos)) res = false;

        if(path == "Create new world") {
            load_state::setState(load_state::Creating_map);
            if(!main_world.generate()) res = false;
        }

        load_state::setState(load_state::Initializing_map);


#if USE_MULTITHREADING
        const int threads = 10;
        std::vector<std::future<void>> futures;


        for(int i = 0; i < threads; i++) {
            int step = worldSize::world_sx / threads;
            
            futures.emplace_back(std::async(std::launch::deferred,[this, i, step]() {
                main_world.initVoxelMap(i * step, (1 + i) * step);
                return;
            }));
        }

        prndd("Waiting for init threads to finnish...");

        for (auto& future : futures) {
            future.wait();
            prndd("Thread finished!");
        }

    
#else 
    main_world.initVoxelMap(0, worldSize::world_sx);
#endif

        weatherManager.load();

        if(path == "Create new world") {
            load_state::setState(load_state::Generating_vegetation);
            if(!main_world.generateVegetation()) res = false;
        } else {
            main_world.loadProcGenData(path);
            
            loadAddWorlds(StorageSettings::save_path + path + "/voxelGroups");
        }

        mobManager.load();
        mobManager.spawnMobs(main_world.procGen, main_world.getChunkIndexer());
        
        load_state::setState(load_state::Unloding_chunks);

        main_world.unloadAllMultithreaded();

        return res;
    }

    void setAddWorldsPointImpact(std::vector<ExplosionInfo>& points) {
        for(auto &world : add_worlds) {
            for(auto &point : points) {
                float distance = math::distance(point.position, world->getPosition());

                if(distance < point.strength) {
                    const sf::Vector2f velocity = -(point.position - world->getPosition()) / 10.f;

                    world->setVelocity(velocity / 1.5f);
                }
            }
        }
    }

    void update(const float dt, Player &player, GameEventEnum::Event& gameEventManager, ShaderEffect& eff) {
        main_world.update(player, gameEventManager, eff);
        mobManager.update(dt, player, main_world);
        mobManager.checkCollisions(main_world);


        sf::Vector2i collPos = sf::Vector2i(
            player.getHeadPosition() + sf::Vector2f(0, PlayerGlobal::characterHitBoxSize.y)
        );

        Voxel& currentVoxel = main_world.getChunkIndexer().boundGetVoxelAt(collPos.x, collPos.y);

        if(PlayerState::currentState == PlayerState::walkState && currentVoxel.value != 0) {
            if(currentVoxel.strenght == 1) 
                GameSoundManager::Organic::walk_dirt.play();

        } else if(PlayerState::currentState == PlayerState::idleState && currentVoxel.value != 0) {
            if(player.getPhysicsComponent().velocity.y > 3.0 && currentVoxel.strenght == 1) {
                GameSoundManager::Organic::fall_soil_hard.play();
                player.getPhysicsComponent().velocity.y = 2.9;
            }
        } else if(PlayerState::currentState == PlayerState::swimState && currentVoxel.value != 0) {
            if(player.getPhysicsComponent().velocity.y > 3.0) {
                
                GameSoundManager::Liquid::splash_water.play();
                player.getPhysicsComponent().velocity.y = 2.9;
                sf::Vector2i chunk_pos = main_world.getChunkIndexer().getChunkFromPos(collPos.x, collPos.y);

                Chunk& chunk = main_world.getChunkIndexer().boundGetChunkAt(chunk_pos.x, chunk_pos.y);

                for (int dx = -2; dx <= 2; ++dx) {
                    for (int dy = -2; dy <= 2; ++dy) {
                        main_world.getChunkIndexer().boundGetChunkAt(chunk_pos.x + dx, chunk_pos.y + dy).needs_update = true;
                    }
                }

                for(auto &e : chunk.elements) {
                    if(math::distance(sf::Vector2f(*e), sf::Vector2f(player.getHeadPosition())) < 10) {
                        e->setVelocity(sf::Vector2i(math::randIntInRange(-3, 3), -4));
                        e->update(main_world.getChunkIndexer());
                    }
                }
            }
        }


        
        // Temporary container to hold the elements to be removed
        std::list<std::shared_ptr<VoxelGroup>> to_be_removed;

        for (const auto &v : AddVoxelGroups::add_more_worlds) {
            add_worlds.emplace_back(v);
            to_be_removed.push_back(v);
        }

        // Remove the elements from add_more_worlds
        for (const auto &v : to_be_removed) {
            AddVoxelGroups::add_more_worlds.remove(v);
        }

        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            (*world)->update(main_world.getChunkIndexer(), dt);
            if(VoxelGroupPropetiesHandler::handle((*world), player)) {
                add_worlds.erase(world);
                break;
            }
        }

        weatherManager.update(main_world, player);
    }

    void handleCollisionsWithPlayer(Player& player) {
        CollisionManager::CollisionState res = CollisionManager::playerHandleCollisions(player.getPhysicsComponent(), main_world.getChunkIndexer(), PlayerGlobal::characterHitBoxSize);
        player_loc_ = &player;

        if(res.hasCollision) {
            player.ground();
        }

        if(res.isLiquidContact) {
            if(PlayerState::currentState != PlayerState::jumpState) {
                PlayerState::currentState = PlayerState::swimState;
                PlayerState::currentState->enter();
            }
        } else {
            if(PlayerState::currentState == PlayerState::swimState) {
                PlayerState::currentState = PlayerState::idleState;
                PlayerState::currentState->enter();
            }
        }
    }

    void save() {
        main_world.save();

        for(auto &world : add_worlds) {
            world->save();
        }
    }   

    bool checkEntityCollisions(const sf::Vector2f &point) {
        if(mobManager.getCollisionsInPoint(point))
            return true;

        if(player_loc_ != nullptr) {
            if(player_loc_->getHitbox().contains(point))
                return true;
        }

        return false;
    }

    bool damageEntityWithCollision(const sf::Vector2f &point, int damage) {
        if(mobManager.damageMobsIfCollision(point, damage))
            return true;

        if(player_loc_ != nullptr) {
            if(player_loc_->getHitbox().contains(point))
                player_loc_->getHealthManager().damageHit(damage);
        }

        return false;
    }

    void mobInvoke(Player& player, std::vector<ExplosionInfo> &points) {
        mobManager.invokeMobs(player, points);
    }

    void render(sf::RenderTarget& target, sf::RenderTarget& rtx, const sf::Vector2f& view_center) {
        main_world.render(target, rtx, view_center);
        mobManager.render(target);

        for(auto &world : add_worlds) {
            world->render(target, view_center);
        }
    }

    void loadAddWorlds(const std::string &path) {
        if(!fs::exists(path)) return;

        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_directory(entry.path())) {
                const std::string& completePath = entry.path().string();

                auto group = std::make_shared<VoxelGroup>();
                group->loadFromStorage(completePath, std::stoi(entry.path().stem().string()));

                add_worlds.emplace_back(group);
            }
        }
    }

	VoxelManager main_world {};
    std::list<std::shared_ptr<VoxelGroup>> add_worlds {};
private:
    Player* player_loc_ = nullptr;

    WeatherManager weatherManager;
    MobManager mobManager;
};