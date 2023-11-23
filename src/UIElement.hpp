#pragma once
#include <SFML/Graphics.hpp>

class UIElement {
    public:
        virtual bool load(const int sizeX, const int sizeY) = 0;
        virtual void render(sf::RenderTarget &target) = 0;
        virtual void update() = 0;
    private:
};