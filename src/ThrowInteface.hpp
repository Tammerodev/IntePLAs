#pragma once
#include <SFML/Graphics.hpp>
#include "Palettes/PaletteUI.hpp"
#include "math.hpp"

class ThrowInteface {
public:
    void enter() {
        throw_ = false;
        active = true;
    }

    void update(const sf::Vector2f& player_pos, const sf::Vector2f& mouse_pos) {
        start_pos = player_pos;
        end_pos = mouse_pos;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            throw_ = true;
        }
    }

    bool isThrowed() {
        return throw_ && active;
    }

    const sf::Vector2f getThrowVelocity() {
        return math::subVector(end_pos, start_pos);
    }

    const sf::Vector2f getThrowPosition() {
        return start_pos;
    }

    void render(sf::RenderTarget &target) {
        if(!active) return;

        const int numCircles = 10; 
        sf::Vector2f delta = (end_pos - start_pos) / (float)numCircles;

        for (int i = 0; i < numCircles; ++i) {
            sf::CircleShape circle(2.0f);
            circle.setFillColor(Palette::PaletteUI::Gray);

            sf::Vector2f circlePos = start_pos + delta * static_cast<float>(i);
            circle.setPosition(circlePos - sf::Vector2f(circle.getRadius(), circle.getRadius()));

            target.draw(circle);
        }
    }

    void exit() {
        active = false;
    }

private:
    bool active = false;

    sf::Vector2f start_pos;
    sf::Vector2f end_pos;
    bool throw_ = false;
};
