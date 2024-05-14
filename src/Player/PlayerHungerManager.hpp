#pragma once
#include "PlayerGlobal.hpp"
#include "PlayerState.hpp"
#include "../SoundFX.hpp"
#include "PlayerDamageSoundType.hpp"
#include "../Camera.hpp"

class PlayerHungerManager {
    public:
        void update() {
            if(hungerClock.getElapsedTime().asSeconds() > 10.0) {
                PlayerGlobal::hunger--; 
                hungerClock.restart();
            } 

            if(PlayerGlobal::hunger < 0) {
                PlayerGlobal::hunger = PlayerGlobal::max_hunger;
            }
        }
    private:
        sf::Clock hungerClock;
};