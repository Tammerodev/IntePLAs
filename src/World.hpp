#pragma once 
#include <list>
#include <algorithm>
#include <iostream>
#include "VoxelManager.hpp"
#include "Player.hpp"
#include "VoxelGroup.hpp"

class World {
public:
    bool init(const std::string path) {
        bool res = true;
        
        if(!main_world.load(path)) res = false;

        if(path == "Create new world") {
            if(!main_world.generate()) res = false;
            if(!main_world.generateVegetation()) res = false;
        }

        main_world.initVoxelMap();
        
        return res;
    }

    void update(const float dt, Player &player) {
        main_world.update(player);
        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            world->update(dt);
            const std::vector<sf::Vector2i> &collisionTestPoints = world->getCollisionTestPoints();

            bool collision = false;

            for(auto &testPoint : collisionTestPoints) {
                if(main_world.getPixelCollision(testPoint)) collision = true;
            }
            
            if(collision) {    
            
                world->getPhysicsComponent().velocity = sf::Vector2f(0.0f, 0.0f);

                world->destroyPart(main_world);

                add_worlds.erase(world);
    
                break;
            }
        }

    }

    void handleCollisionsWithPlayer(Player& player) {

        IntPhysicsComponent &player_physicscomp = player.getPhysicsComponent();

        const bool groundCollision = main_world.getPixelCollision(sf::Vector2i(player_physicscomp.transform_position.x,
                                                            player_physicscomp.transform_position.y + 28));

        const bool headCollision = main_world.getPixelCollision(sf::Vector2i(player_physicscomp.transform_position.x,
                                                            player_physicscomp.transform_position.y - 3)); 

        const bool rightCollision = main_world.getPixelCollision(sf::Vector2i(player_physicscomp.transform_position.x + 20,
                                                            player_physicscomp.transform_position.y + 14)); 

        const bool leftCollision = main_world.getPixelCollision(sf::Vector2i(player_physicscomp.transform_position.x - 3,
                                                            player_physicscomp.transform_position.y + 14));

        const bool sideCollision = rightCollision || leftCollision;

        if(sideCollision) {
            player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;
        }

        if(groundCollision) {
            player_physicscomp.transform_position.y -= 1;
            player.ground();
        }

        if(headCollision) {
            player_physicscomp.velocity.y = 0;
            player_physicscomp.velocity_buffer = 0;

            player_physicscomp.transform_position.y += 1;
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

};