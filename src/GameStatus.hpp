#pragma once
#include <SFML/System.hpp>
#include <cmath>

namespace GameStatus {
    inline float brightness = 0.0;

    inline sf::Clock game_clock;
    
    inline void updateBrightness() {
        const float timeCycle = 12.0;
        brightness = sin(game_clock.getElapsedTime().asSeconds() / timeCycle) * 3.0;
    
    }

}