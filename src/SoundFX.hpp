#pragma once
#include <SFML/Audio.hpp>

namespace SFX {
    inline sf::SoundBuffer strong_explosion_buffer;
    inline sf::SoundBuffer rocket_launcher_fire_buffer;
    inline sf::SoundBuffer fire_buffer;
    inline sf::SoundBuffer damage_buffer;
    inline sf::SoundBuffer geigerCounter_buffer;
    inline sf::SoundBuffer rocket_buffer;
    inline sf::SoundBuffer pickaxe_swing_buffer;
    inline sf::SoundBuffer collect_buffer;
    inline sf::SoundBuffer nuclear_explosion_buffer;


    inline sf::Sound strong_explosion;
    inline sf::Sound rocket_launcher_fire;
    inline sf::Sound fire;
    inline sf::Sound damage;
    inline sf::Sound geigerCounter;
    inline sf::Sound rocket;
    inline sf::Sound pickaxe_swing;
    inline sf::Sound collect;
    inline sf::Sound nuclear_explosion;

    
    inline int load() {
        int res = 1;
        // Load buffers
        if(!rocket_launcher_fire_buffer.loadFromFile("res/sound/rocket_launcher_fire.wav")) res = 0;
        if(!strong_explosion_buffer.loadFromFile("res/sound/strong_explosion.wav")) res = 0;
        if(!fire_buffer.loadFromFile("res/sound/fire.wav")) res = 0;
        if(!damage_buffer.loadFromFile("res/sound/damage.wav")) res = 0;
        if(!geigerCounter_buffer.loadFromFile("res/sound/geiger_counter.wav")) res = 0;
        if(!rocket_buffer.loadFromFile("res/sound/rocket.wav")) res = 0;
        if(!pickaxe_swing_buffer.loadFromFile("res/sound/pickaxe_swing.wav")) res = 0;
        if(!collect_buffer.loadFromFile("res/sound/collect.wav")) res = 0;
        if(!nuclear_explosion_buffer.loadFromFile("res/sound/nuclear_explosion.wav")) res = 0;
        

        // Set buffers
        strong_explosion.setBuffer(strong_explosion_buffer);
        rocket_launcher_fire.setBuffer(rocket_launcher_fire_buffer);
        fire.setBuffer(fire_buffer);
        damage.setBuffer(damage_buffer);
        geigerCounter.setBuffer(geigerCounter_buffer);
        rocket.setBuffer(rocket_buffer);
        pickaxe_swing.setBuffer(pickaxe_swing_buffer);
        collect.setBuffer(collect_buffer);
        nuclear_explosion.setBuffer(nuclear_explosion_buffer);
        
        return res;
    }
}