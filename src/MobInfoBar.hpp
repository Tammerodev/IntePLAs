#pragma once
#include <SFML/Graphics.hpp>
#include "TextLabel.hpp"

class MobInfoBar {
    public:
        void load() {
            textLabel.load();
            textLabel.setFontSize(10);
        }

        void render(sf::RenderTarget& target) {
            textLabel.render(target);
        }

        void setPosition(const sf::Vector2f& pos) {
            textLabel.setPosition(pos);
        }

        void update(const std::string& mob_name, const int mob_health) {
            const std::string text = mob_name + " ; Health " + std::to_string(mob_health);

            textLabel.setText(text);
        }

    private:
        TextLabel textLabel;
};