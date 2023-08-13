#pragma once 
#include "VoxelManager.hpp"
#include "Player.hpp"
#include "VoxelGroup.hpp"

class World {
public:
    bool init(const std::string path) {
        bool res = true;
        
        if(!main_world.load(path, path == "res/saves/Create new world.png")) res = false;
        if(!main_world.generate()) res = false;
        //if(!main_world.generateVegetation()) res = false;

        main_world.initVoxelMap();
        
        return res;
    }

    void update() {
        main_world.update();
        for (auto world = add_worlds.begin(); world != add_worlds.end(); ++world) {
            world->update();
            std::pair<bool, sf::FloatRect> collision_rect = main_world.getOvelapWithRect(world->getCollider());
            if(collision_rect.first) {
                world->getPhysicsComponent().velocity = sf::Vector2f(0.0f, 0.0f);
                world->getPhysicsComponent().transform_position.y -= collision_rect.second.height;

                world->destroyPart(main_world, collision_rect.second);
            }
        }

    }

    void handleCollisionsWithPlayer(Player& player) {
        auto res = main_world.getOvelapWithRectY(player.getBottomHitbox()); // Ground
        auto res2 = main_world.getOvelapWithRect(player.getTopHitbox()); // Ground
        auto res4 = main_world.getOvelapWithRect(player.getRightHitbox()); // Ground
        auto res3 = main_world.getOvelapWithRect(player.getLeftHitbox()); // Ground

        if(res4.first) {    // Right collision
            player.getPhysicsComponent().transform_position.x -= res4.second.width + 2;
        }
        if(res3.first) {    // Right collision
            player.getPhysicsComponent().transform_position.x += res3.second.width + 2;
        }
        if(res.first) {     // Colliding with ground
            player.ground();
            player.getPhysicsComponent().transform_position.y -= res.second.height;
            return;
        }

        if(res2.first) {    // Head collision
            player.getPhysicsComponent().transform_position.y += res2.second.height;
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
    std::vector<VoxelGroup> add_worlds {};
private:

};