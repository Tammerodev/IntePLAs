#pragma once
#include <SFML/System.hpp>
#include <cmath>

namespace GameStatus {
    inline float brightness = 0.0;

    inline double game_clock = 0.0; 

    inline void updateBrightness() {
        const float timeSpeed = 12.0;
        game_clock += 1.0 / 75.0; 
        brightness = sin(game_clock / timeSpeed) * 3.0;
    }

}