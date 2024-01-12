#pragma once
#include "GameEvent.hpp"

class NuclearExplosionGameEvent : public GameEvent {
    public:
        void enter() {
            clock.restart();
            time = 0.f;

            flash.setSize(sf::Vector2f(2000, 2000));
            flash.setPosition(0, 0);
        }

        void update(VoxelManager& vx) {
            time = clock.getElapsedTime().asMilliseconds();

            if(time > 1000) {
                flash_opacity -= 2;
                if(flash_opacity < 0) 
                    flash_opacity = 0;
            }
            
            flash.setFillColor(sf::Color(flash.getFillColor().r, flash.getFillColor().g, flash.getFillColor().b, flash_opacity));
        }

        void render(sf::RenderTarget& target) {
            target.draw(flash);
        }

    private:

        sf::RectangleShape flash;
        int flash_opacity = 255;

        float time = 0.f;
        sf::Clock clock;
};