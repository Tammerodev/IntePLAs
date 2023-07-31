#pragma once 
#include "VoxelManager.hpp"
#include "Entity/Player/Player.hpp"
#include "VoxelGroup.hpp"

class World {
public:
    bool init(const std::string path) {
        bool res = true;
        
        if(!main_world.load(path, path == "res/saves/Create new world.png")) res = false;

        sf::Image image;
        image.loadFromFile("res/img/Tool/Heatgun.png");

        add_world.load("res/img/Tool/Heatgun.png", false);

        return res;
    }

    void update() {
        main_world.update();
        add_world.update();
    }

    void handleCollisionsWithPlayer(Player& player) {
        auto res = main_world.getOvelapWithRectY(player.getBottomHitbox()); // Ground
        auto res2 = main_world.getOvelapWithRect(player.getTopHitbox()); // Ground
        auto res4 = main_world.getOvelapWithRect(player.getRightHitbox()); // Ground
        auto res3 = main_world.getOvelapWithRect(player.getLeftHitbox()); // Ground

        if(res4.first) {    // Right collision
            player.move_x(-(res4.second.width + 2));
        }
        if(res3.first) {    // Right collision
            player.move_x((res3.second.width + 2));
        }
        if(res.first) {     // Colliding with ground
            player.ground();
            player.move_y(-res.second.height);
            return;
        }

        if(res2.first) {    // Head collision
            player.move_y(res2.second.height);
        }
    }

    void save() {
        main_world.save();
    }

    void render(sf::RenderTarget& target, const sf::Vector2f& view_center) {
        main_world.render(target, view_center);
        add_world.render(target, view_center);
    }

	VoxelManager main_world {};
    VoxelGroup add_world {};
private:

};