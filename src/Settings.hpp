#pragma once
#include <SFML/Graphics/Color.hpp>
#include "JsonManager.hpp"
#include <assert.h>
#include "common.hpp"

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

    inline bool loadGraphicsSettings() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/graphicssettings.json");
        result = reader.init();

        const std::string useParticleEffects = reader.readParameterAsString("use-particle-effects");

        if(useParticleEffects == "true") particleEffects = true;
        else if(useParticleEffects == "false") particleEffects = false;
        else result = false;

        return result;
    }
}

namespace WorldSettings {
    inline unsigned int createSizeX = 0;
    inline unsigned int createSizeY = 0;
    inline bool worldConfigLoaded = false;

    inline bool loadWorldConfig() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/worldconfig.json");
        result = reader.init();

        const std::string world_size_x = reader.readParameterAsString("world-size-x");
        const std::string world_size_y = reader.readParameterAsString("world-size-y");

        createSizeX = std::stoi(world_size_x);
        createSizeY = std::stoi(world_size_y);

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

    inline bool loadSettings() {
        bool result = false;

        JsonReader reader;
        result = reader.open("json/storagesettings.json");
        result = reader.init();

        save_path = reader.readParameterAsString("save-path");

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
        
        if(!StorageSettings::loadSettings()) result = false;

        loginf("Loaded data from JSON : save path = ", StorageSettings::save_path, ".");

        return result;
    }
}