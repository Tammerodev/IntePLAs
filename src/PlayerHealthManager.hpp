#pragma once
#include "PlayerGlobal.hpp"
#include "PlayerState.hpp"
#include "SoundFX.hpp"
#include "PlayerDamageSoundType.hpp"
#include "Camera.hpp"

class PlayerHealthManager {
    public:
        void calculateHeatDamage(const float temp, Camera &cam) {
            if(temp > 120.0) {
                if(math::randProp(2))
                    damagePlayer(1, PlayerDamageSoundType::Hot, false);
            }

            if(temp < -20.0) {
                cam.shake(temp / -0.1);
            } if(temp < -27.0) {
                damagePlayer(1, PlayerDamageSoundType::Cold, false);
            }
        }

        void damageRadiation(const int damaged);

        void damageHit(const int damaged);

    private:
        void damagePlayer(const int damage, const PlayerDamageSoundType soundType, bool goToDamageState = true) {
            SFX::DamageSounds::playDamageSoundOfType(soundType);
            PlayerGlobal::health -= damage;

            if(goToDamageState) {
                PlayerState::currentState = PlayerState::damageState;
                PlayerState::currentState->enter();
            }

            checkPlayerHealth();
        }

        void checkPlayerHealth() {
            if(PlayerGlobal::health <= 0) {
                PlayerState::currentState = PlayerState::dead;
                PlayerState::currentState->enter();

                PlayerGlobal::health = 0;
            }
        };
};