#pragma once
#include "common.hpp"
#include <SFML/Graphics.hpp>

class Panel : public sf::Sprite {
public:
    Panel() {}
    Panel(const std::string& path) {
        if(!tx.loadFromFile(path)) prnerr("Could not load file at ", path);
        setTexture(tx);
    }

    void applyTexture() {
        setTexture(tx);
    }

    sf::Texture &getTextureRef() {
        return tx;
    }

private:
    sf::Texture tx;
};