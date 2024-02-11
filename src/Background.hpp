#pragma once
#include <SFML/Graphics.hpp>
#include "ParallaxLayer.hpp"
#include "common.hpp"
#include "Shader.hpp"
#include "VoxelManager.hpp"

class Background {
public:
    int load() {
        createBackgrounds();
        changeBackgrounds("city");
		return 1;
    }

    void createBackgrounds() {
        parallax.at(0).create(0.10f);
        parallax.at(1).create(0.25f);
        parallax.at(2).create(0.50f);
        parallax.at(3).create(0.75f);
        parallax.at(4).create(1.00f);
    }

    void changeBackgrounds(const std::string &name) {
        if(name != currentBiome) {
            parallax.at(0).load("res/world/Parallax/" + name + "/1.png");
            parallax.at(1).load("res/world/Parallax/" + name + "/2.png");
            parallax.at(2).load("res/world/Parallax/" + name + "/3.png");
            parallax.at(3).load("res/world/Parallax/" + name + "/4.png");
            parallax.at(4).load("res/world/Parallax/" + name + "/5.png");
        }

        currentBiome = name;
    }
    

    void render(sf::RenderTarget &targ) {
        for(auto &layer : parallax) {
            layer.render(targ);
        }
    }

    void update(sf::View &view, const float dt) {
        for(auto &layer : parallax) {
            layer.update(view);
        }
    }

    void updateForBiomeChanges(Player &player, VoxelManager &vx) {
        // tODO
        /*Biome &biome = vx.procGen.getBiomeAtPosition(player.getPhysicsComponent().transform_position.x, vx.getChunkIndexer());

        const std::string background_name = biome.getName();

        changeBackgrounds(background_name);*/
    }

private:
    std::array<ParallaxLayer, 5> parallax;
	sf::Clock shader_time;
	
    std::string currentBiome = "";

    int time_in_biome = 0;
};