#pragma once
#include "HeistMod/HeistMod.hpp"

inline std::vector<std::shared_ptr<CustomGameMod>> modsInUse {
    std::make_shared<HeistMod>(),
};