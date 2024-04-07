#pragma once 
#include <list>
#include <algorithm>
#include <iostream>
#include "VoxelManager.hpp"
#include "Player.hpp"
#include "VoxelGroup.hpp"
#include "WeatherManager.hpp"

#include "CollisionManager.hpp"
#include "MobManager.hpp"

class World {
public:
    bool init(const std::string path) {
        bool res = true;
        
        load_state::setState(load_state::Loading_map);
        if(!main_world.load(path)) res = false;

        if(path == "Create new world") {
            load_state::setState(load_state::Creating_map);
            if(!main_world.generate()) res = false;
        }

        load_state::setState(load_state::Initializing_map);
        main_world.initVoxelMap();

        weatherManager.load();

        if(path == "Create new world") {
            load_state::setState(load_state::Generating_vegetation);
            if(!main_world.generateVegetation()) res = false;
        } else {
            main_world.loadProcGenData(path);
        }

        mobManager.load();
        mobManager.spawnMobs(main_world.procGen, main_world.getChunkIndexer());
        
        load_state::setState(load_state::Unloding_chunks);
        main_world.unloadAll();


        return res;
    }

    void update(const float dt, Player &player, GameEventEnum::Event& gameEventManager) {
        main_world.update(player, gameEventManager);
        mobManager.update(dt, player, main_world);
        mobManager.checkCollisions(main_world);

        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            world->update(main_world.getChunkIndexer(), dt);
            const std::vector<sf::Vector2i> &collisionTestPoints = world->getCollisionTestPoints();

            bool collision = false;

            for(auto &testPoint : collisionTestPoints) {
                if(main_world.getChunkIndexer().getPixelCollision(sf::Vector2f(testPoint)).first) collision = true;
            }
            
            if(collision) {    
            
                world->getPhysicsComponent().velocity = sf::Vector2f(0.0f, 0.0f);

                world->destroyPart(main_world);

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

    void render(sf::RenderTarget& target, const sf::Vector2f& view_center) {
        main_world.render(target, view_center);
        mobManager.render(target);

        for(auto &world : add_worlds) {
            world.render(target, view_center);
        }
    }

	VoxelManager main_world {};
    std::list<VoxelGroup> add_worlds {};
private:
    Player* player_loc_ = nullptr;

    WeatherManager weatherManager;
    MobManager mobManager;
};