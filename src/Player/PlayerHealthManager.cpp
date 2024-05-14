#include "PlayerHealthManager.hpp"

void PlayerHealthManager::damageRadiation(const int damaged) {
    PlayerGlobal::health -= damaged;

    if(PlayerGlobal::health <= 0) {
        PlayerState::currentState = PlayerState::dead;
        PlayerState::currentState->enter();

        PlayerGlobal::health = 0;
    }
}

void PlayerHealthManager::damageHit(const int damaged) {
    damagePlayer(damaged, PlayerDamageSoundType::Hit);
}
