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
            text.setFont(UI::font);
        }

        void render(sf::RenderTarget& target) {
            text.setPosition(getPosition());

            target.draw(*this);
            target.draw(text);
        }

        void setAmount(int x, int y) {
            setSize(sf::Vector2f(x, y));
            text.setString(std::to_string(y));
        }
    private:
        sf::Text text;
};