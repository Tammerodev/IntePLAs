#pragma once
#include <SFML/Graphics.hpp>

namespace UI {
    inline sf::Font font;

    inline void loadFont() {
        font.loadFromFile("res/Fonts/VT323.ttf");
    }
}

class MaterialBar : public sf::RectangleShape {
    public:
        MaterialBar() {

            prndd("Loading FIHwtaawi0tjpaowithapowithp!");

            text.setCharacterSize(15);
            text.setString("TEST");

            font.loadFromFile("res/Fonts/VT323.ttf");
            text.setFont(font);
        }

        void _setPosition(float x, float y) {
            text.setPosition(x, y);
            setPosition(x, y);
        }

        void render(sf::RenderTarget& target) {

            target.draw(*this);
            target.draw(text);
        }

        void setAmount(int x, int y) {
            setSize(sf::Vector2f(x, y));
            text.setString("::");
        }
    private:
        sf::Text text;
        sf::Font font;
};