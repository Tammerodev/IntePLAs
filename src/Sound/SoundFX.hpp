#pragma once
#include <SFML/Audio.hpp>

namespace SFX {
    static sf::SoundBuffer strong_explosion_buffer;
    static sf::SoundBuffer rocket_launcher_fire_buffer;
    static sf::SoundBuffer fire_buffer;

    static sf::Sound strong_explosion;
    static sf::Sound rocket_launcher_fire;
    static sf::Sound fire;


    
    static int load() {
        int res = 1;
        // Load buffers
        if(!rocket_launcher_fire_buffer.loadFromFile("res/sound/rocket_launcher_fire.wav")) res = 0;
        if(!strong_explosion_buffer.loadFromFile("res/sound/strong_explosion.wav")) res = 0;
        if(!fire_buffer.loadFromFile("res/sound/fire.wav")) res = 0;
        

        // Set buffers
        strong_explosion.setBuffer(strong_explosion_buffer);
        rocket_launcher_fire.setBuffer(rocket_launcher_fire_buffer);
        fire.setBuffer(fire_buffer);

        
        return res;
    }
}