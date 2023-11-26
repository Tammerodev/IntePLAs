#pragma once
#include <SFML/System.hpp>
#include <cmath>

namespace GameStatus {
    inline float brightness = 0.0;

    inline sf::Clock game_clock;
    
    inline void updateBrightness() {
        const float timeCycle = 48.0;
        brightness = 0.5 + sin(game_clock.getElapsedTime().asSeconds() / timeCycle) - 2.0;
    
    }

}