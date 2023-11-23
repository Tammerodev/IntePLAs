#pragma once
#include "UIElement.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>

class BarUI : public UIElement {
    public:
        bool load(const int sizeX, const int sizeY) {
            bar_area.setPosition(sf::Vector2f(sizeX / 3, sizeY - (index * 16)));
            bar_filled.setPosition(bar_area.getPosition());

            return true;
        }

        void update() {
            bar_filled.setFillColor(color_fill);
            bar_filled.setSize(sf::Vector2f(value, 16));

            bar_area.setFillColor(color_empty);
            bar_area.setSize(sf::Vector2f(max_value, 16));
        }

        void render(sf::RenderTarget &target) {
            target.draw(bar_area);
            target.draw(bar_filled);
        }

        void setValue(const int val) {
            value = val;
        }

        void setMaxValue(const int max_val) {
            max_value = max_val;
        }

        void setColorFill(const sf::Color& color) {
            color_fill = color;
        } 

        void setColorEmpty(const sf::Color& color) {
            color_empty = color;
        }

        void setIndex(const int ind) {
            index = ind;
        }
        
    private:
        int value = 0;
        int max_value = 0;

        sf::Color color_fill;
        sf::Color color_empty;

        int index = 0;

        sf::RectangleShape bar_area;
        sf::RectangleShape bar_filled;
};