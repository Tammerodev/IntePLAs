#pragma once
#include <SFML/Audio.hpp>

namespace SFX {
    static sf::SoundBuffer amplified_explosion_buffer;
    static sf::Sound amplified_explosion;

    
    static int load() {
        int res = 1;
        // Load buffers
        if(!amplified_explosion_buffer.loadFromFile("res/sound/amplified_explosion.wav")) res = 0;
        // Set buffers
        amplified_explosion.setBuffer(amplified_explosion_buffer);

        return res;
    }
}