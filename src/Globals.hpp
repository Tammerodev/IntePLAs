#pragma once
#include <SFML/Graphics/Sprite.hpp>

typedef unsigned long long uFrame;

namespace Globals {
    inline sf::Sprite* renderSprite = nullptr; 

    inline unsigned long long frame = 0;

    inline void resetFrame() {
        frame = 0;
    }
}