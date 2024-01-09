#pragma once
#include <SFML/Graphics.hpp>

namespace Fonts {
    inline sf::Font font_pixelated;

    inline bool loadFont() {
        return font_pixelated.loadFromFile("res/Fonts/VT323.ttf");
    }

}