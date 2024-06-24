#pragma once
#include "Sound.hpp"

namespace GameSoundManager::Paths {
    inline std::string base = "res/sound/MaterialSounds/";
}

namespace GameSoundManager::Organic {
    inline Sound walk_dirt      = {Paths::base + "untitled_hard.wav"};
    inline Sound walk_plant     = {Paths::base + "untitled_hard.wav"};

    inline Sound destroy_dirt   = {Paths::base + "untitled_hard.wav"};
    inline Sound destroy_plant  = {Paths::base + "untitled_hard.wav"};

    inline Sound fall_soil_hard = {Paths::base + "Organic/fall_soil_hard.wav"};
}

namespace GameSoundManager::Liquid {
    inline Sound splash_water      = {Paths::base + "Liquid/splash_water_hard.wav"};
}