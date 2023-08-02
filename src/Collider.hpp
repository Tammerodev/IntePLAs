#pragma once
#include <SFML/Graphics.hpp>

class Collider : public sf::FloatRect {
public:
    void debugdraw(sf::RenderTarget& target) {
        visual_rectangle.setPosition(left, top);
        visual_rectangle.setSize(sf::Vector2f(width, height));
        visual_rectangle.setOutlineColor(sf::Color::Red);
        visual_rectangle.setFillColor(sf::Color::White);
        visual_rectangle.setOutlineThickness(3);

        target.draw(visual_rectangle);
    }
private:
    sf::RectangleShape visual_rectangle;
};