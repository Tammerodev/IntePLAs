#pragma once
#include <SFML/Graphics.hpp>
#include "FontManager.hpp"

class TextLabel {
    public:
        void load() {
            text.setFont(Fonts::font_pixelated);
        }

        void setFontSize(const unsigned int size) {
            text.setCharacterSize(size);
        }

        void render(sf::RenderTarget& target) {
            target.draw(text);
        }

        void setPosition(const sf::Vector2f& position) {
            text.setPosition(position);
        }

        void setText(const std::string& text_string) {
            text.setString(text_string);
        }
    private:
        sf::Text text;
};