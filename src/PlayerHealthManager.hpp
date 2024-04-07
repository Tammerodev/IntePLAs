#pragma once
#include "PlayerGlobal.hpp"
#include "PlayerState.hpp"
#include "SoundFX.hpp"
#include "PlayerDamageSoundType.hpp"

class PlayerHealthManager {
    public:
        void calculateHeatDamage(const float temp) {
            if(temp > 120.0) {
                if(math::randProp(2))
                    damagePlayer(std::clamp(temp / 150.f, 0.0f, 3.0f), PlayerDamageSoundType::Hot, false);
            }

            if(temp < -27.0) {
                if(math::randProp(2))
                    damagePlayer(std::clamp(temp / -10.f, 0.0f, 3.0f), PlayerDamageSoundType::Cold, false);
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