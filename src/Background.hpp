#pragma once
#include <SFML/Graphics.hpp>
#include "ParallaxLayer.hpp"
#include "common.hpp"
#include "Shader.hpp"

class Background {
public:
    int load() {
        parallax.push_back(ParallaxLayer("res/world/forest_bg_layer2.png", 0.05f));
        //parallax.push_back(ParallaxLayer("res/world/forest_bg_layer1.png", 0.1f));
        //parallax.push_back(ParallaxLayer("res/world/forest_bg_layer0.png", 0.2f));

		return 1;
    }

    void render(sf::RenderTarget &targ) {
        for(auto &layer : parallax) {
            layer.render(targ);
        }
    }

    void update(sf::Vector2f pos) {
        for(auto &layer : parallax) {
            layer.update(pos);
        }
    }

private:
    std::vector<ParallaxLayer> parallax;
	sf::Clock shader_time;
	

};