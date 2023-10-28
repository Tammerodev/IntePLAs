#pragma once
#include <SFML/Graphics/Color.hpp>
#include "JsonManager.hpp"
#include <assert.h>

namespace UISettings {
    inline unsigned char transparency = 100;
    inline sf::Color colorRGB = sf::Color(187, 212, 206);

    inline const sf::Color getColorRGBA() {
        return sf::Color(
            colorRGB.r, colorRGB.g, colorRGB.b, transparency
        );
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

        prndd(world_size_x);
        prndd(world_size_y);

        createSizeX = std::stoi(world_size_x);
        createSizeY = std::stoi(world_size_y);


        prndd(createSizeX);
        prndd(createSizeY);

        worldConfigLoaded = result;
        return worldConfigLoaded;
    }
}