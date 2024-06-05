#pragma once
#include "PlayerGlobal.hpp"
#include "PlayerState.hpp"
#include "../SoundFX.hpp"
#include "PlayerDamageSoundType.hpp"
#include "../Camera.hpp"

class PlayerHungerManager {
    public:
        void update(PlayerHealthManager &phm) {
            if(hungerClock.getElapsedTime().asSeconds() > 10.0) {
                PlayerGlobal::hunger--; 
                hungerClock.restart();
            } 

            if(PlayerGlobal::hunger < 0) {
                phm.damageHit(1);
            }
        }

        void eat(const unsigned int nutrition) {
            PlayerGlobal::hunger += nutrition;
        }
    private:
        sf::Clock hungerClock;
};