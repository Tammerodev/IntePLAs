#pragma once
#include <SFML/Graphics/Color.hpp>
#include "JsonManager.hpp"
#include <assert.h>
#include "common.hpp"
#include "Utils/StringUtils.hpp"
#include "FastNoiseLite.hpp"

namespace UISettings {
    inline unsigned char transparency = 100;
    inline sf::Color colorRGB = sf::Color(187, 212, 206);

    inline const sf::Color getColorRGBA() {
        return sf::Color(
            colorRGB.r, colorRGB.g, colorRGB.b, transparency
        );
    }
}

namespace GraphicsSettings {
    inline bool particleEffects = true;
    inline unsigned int max_fps = 75;
    inline bool use_vsync = false;

    inline bool loadGraphicsSettings() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/graphicssettings.json");
        result = reader.init();

        particleEffects = StringUtils::stringToBool(reader.readParameterAsString("use-particle-effects"));
        use_vsync = StringUtils::stringToBool(reader.readParameterAsString("use-vsync"));

        max_fps = std::stoi(reader.readParameterAsString("max-fps"));

        return result;
    }
}

namespace WorldSettings {
    inline unsigned int createSizeX = 0;
    inline unsigned int createSizeY = 0;
    inline bool worldConfigLoaded = false;

    inline FastNoiseLite::NoiseType noiseType;

    inline bool loadWorldConfig() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/worldconfig.json");
        result = reader.init();

        const std::string world_size_x = reader.readParameterAsString("world-size-x");
        const std::string world_size_y = reader.readParameterAsString("world-size-y");
        const std::string noise_type   = reader.readParameterAsString("noise-type");

        createSizeX = std::stoi(world_size_x);
        createSizeY = std::stoi(world_size_y);

        if(noise_type == "perlin")
            noiseType = FastNoiseLite::NoiseType::NoiseType_Perlin;
        else if(noise_type == "opensimplex2")
            noiseType = FastNoiseLite::NoiseType::NoiseType_OpenSimplex2;
        else if(noise_type == "opensimplex2S")
            noiseType = FastNoiseLite::NoiseType::NoiseType_OpenSimplex2S;
        else if(noise_type == "value")
            noiseType = FastNoiseLite::NoiseType::NoiseType_Value;
        else if(noise_type == "valuecubic")
            noiseType = FastNoiseLite::NoiseType::NoiseType_ValueCubic;
        else if(noise_type == "cellular")
            noiseType = FastNoiseLite::NoiseType::NoiseType_Cellular;

        worldConfigLoaded = result;
        return worldConfigLoaded;
    }
}

namespace Display {
    inline int window_height = 0;
    inline int window_width = 0;

}

namespace StorageSettings {
    inline std::string save_path;
    inline std::string screenshot_path;


    inline bool loadSettings() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/storagesettings.json");
        result = reader.init();

        save_path = reader.readParameterAsString("save-path");
        screenshot_path = reader.readParameterAsString("screenshot-path");

        return result;
    }
}

namespace SoundSettings {
    inline int music_volume;

    inline bool loadSettings() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/soundsettings.json");
        result = reader.init();

        music_volume = std::stoi(reader.readParameterAsString("music-volume"));

        return result;
    }
}

namespace SettingsLoader {
    inline bool loadSettings() {
        prndd("Loading settings...");

        bool result = true;

        if(!WorldSettings::loadWorldConfig()) result = false;

        loginf("Loaded data from JSON : createsizex = ", WorldSettings::createSizeX, ".");
        loginf("Loaded data from JSON : createsizey = ", WorldSettings::createSizeY, ".");

        if(!GraphicsSettings::loadGraphicsSettings()) result = false;

        loginf("Loaded data from JSON : useparticles = ", GraphicsSettings::particleEffects, ".");
        loginf("Loaded data from JSON : usevsync = ", GraphicsSettings::use_vsync, ".");
        loginf("Loaded data from JSON : max fps = ", GraphicsSettings::max_fps, ".");
        
        
        if(!StorageSettings::loadSettings()) result = false;

        loginf("Loaded data from JSON : save path = ", StorageSettings::save_path, ".");

        if(!SoundSettings::loadSettings()) result = false;

        loginf("Loaded data from JSON : save path = ", SoundSettings::music_volume, ".");

        return result;
    }
}
