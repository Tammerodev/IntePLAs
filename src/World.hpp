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
        
        if(!main_world.load(path, path == "res/saves/Create new world.png")) res = false;
        if(!main_world.generate()) res = false;
        if(!main_world.generateVegetation()) res = false;

        main_world.initVoxelMap();
        
        return res;
    }

    void update() {
        main_world.update();
        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            world->update();
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
        const bool groundCollision = main_world.getPixelCollision(sf::Vector2i(player.getPhysicsComponent().transform_position.x,
                                                            player.getPhysicsComponent().transform_position.y + 28));

        const bool headCollision = main_world.getPixelCollision(sf::Vector2i(player.getPhysicsComponent().transform_position.x,
                                                            player.getPhysicsComponent().transform_position.y - 3)); 

        const bool rightCollision = main_world.getPixelCollision(sf::Vector2i(player.getPhysicsComponent().transform_position.x + 20,
                                                            player.getPhysicsComponent().transform_position.y + 14)); 

        const bool leftCollision = main_world.getPixelCollision(sf::Vector2i(player.getPhysicsComponent().transform_position.x - 3,
                                                            player.getPhysicsComponent().transform_position.y + 14));


        if(rightCollision) {
            player.getPhysicsComponent().transform_position.x -= 1;
        }
        if(leftCollision) {
            player.getPhysicsComponent().transform_position.x += 1;
        }
        if(groundCollision) {
            player.ground();
            player.getPhysicsComponent().velocity.y = 0;
            player.getPhysicsComponent().transform_position.y -= 1;
            return;
        }

        if(headCollision) {
            player.getPhysicsComponent().transform_position.y += 1;
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