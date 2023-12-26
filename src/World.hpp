#pragma once 
#include <list>
#include <algorithm>
#include <iostream>
#include "VoxelManager.hpp"
#include "Player.hpp"
#include "VoxelGroup.hpp"
#include "WeatherManager.hpp"

#include "CollisionManager.hpp"

class World {
public:
    bool init(const std::string path) {
        bool res = true;
        
        if(!main_world.load(path)) res = false;

        if(path == "Create new world") {
            if(!main_world.generate()) res = false;
        }

        main_world.initVoxelMap();

        weatherManager.load();

        if(path == "Create new world") 
            if(!main_world.generateVegetation()) res = false;
        
        return res;
    }

    void update(const float dt, Player &player) {
        main_world.update(player);
        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            world->update(dt);
            const std::vector<sf::Vector2i> &collisionTestPoints = world->getCollisionTestPoints();

            bool collision = false;

            for(auto &testPoint : collisionTestPoints) {
                if(main_world.getPixelCollision(sf::Vector2f(testPoint)).first) collision = true;
            }
            
            if(collision) {    
            
                world->getPhysicsComponent().velocity = sf::Vector2f(0.0f, 0.0f);

                world->destroyPart(main_world);

                add_worlds.erase(world);
    
                break;
            }
        }

        //weatherManager.update(main_world, player);
    }

    void handleCollisionsWithPlayer(Player& player) {
        CollisionManager::CollisionState res = CollisionManager::handleCollisionsWith(player.getPhysicsComponent(), main_world, PlayerGlobal::characterHitBoxSize);

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

    void render(sf::RenderTarget& target, const sf::Vector2f& view_center) {
        main_world.render(target, view_center);
        for(auto &world : add_worlds) {
            world.render(target, view_center);
        }
    }

	VoxelManager main_world {};
    std::list<VoxelGroup> add_worlds {};
private:
    WeatherManager weatherManager;

};