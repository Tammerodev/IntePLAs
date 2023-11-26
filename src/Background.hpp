#pragma once
#include <SFML/Graphics.hpp>
#include "ParallaxLayer.hpp"
#include "common.hpp"
#include "Shader.hpp"

class Background {
public:
    int load() {
        parallax.at(0).create("res/world/Parallax/1.png", 0.10f);
        parallax.at(1).create("res/world/Parallax/2.png", 0.25f);
        parallax.at(2).create("res/world/Parallax/3.png", 0.50f);
        parallax.at(3).create("res/world/Parallax/4.png", 0.75f);
        parallax.at(4).create("res/world/Parallax/5.png", 1.00f);

		return 1;
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

private:
    std::array<ParallaxLayer, 5> parallax;
	sf::Clock shader_time;
	

};