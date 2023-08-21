#pragma once
#include <SFML/Graphics.hpp>

class Cursor : public sf::Sprite {
public:
    bool load() {
        bool res = tx.loadFromFile("res/themes/cross.png");
        setTexture(tx);

        return res;
    }
private:
    sf::Texture tx;    
};