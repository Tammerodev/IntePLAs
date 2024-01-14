#pragma once
#include "GameEvent.hpp"
#include "common.hpp"
#include <iostream>

class NuclearExplosionGameEvent : public GameEvent {
    public:
        void enter() {
            clock.restart();
            time = 0.f;
            flash_opacity = 255;

            flash.setSize(sf::Vector2f(2000, 2000));
            flash.setPosition(0, 0);
            flash.setFillColor(sf::Color::White);

            heatwave_happened = false;
            explosion_happened = false;
        }

        void update(VoxelManager& vx) {
            time = clock.getElapsedTime().asMilliseconds();

            if(time > 100 && heatwave_happened == false) {
                //vx.hole(pos, 2500, false, 20000, 0);

                heatwave_happened = true;
            }

            if(time > 1000) {
                flash_opacity -= 2;
                if(flash_opacity < 0) 
                    flash_opacity = 0;
            }

            if(time > 2500 && explosion_happened == false) {
                vx.holeRayCast(pos, 2000, true, 10000, 0, true);
                explosion_happened = true;
            }



            flash.setFillColor(sf::Color(flash.getFillColor().r, flash.getFillColor().g, flash.getFillColor().b, flash_opacity));
        }

        void render(sf::RenderTarget& target) {
            target.draw(flash);
        }

    private:
        sf::Vector2i pos {5000, 500};

        bool explosion_happened = false;
        bool heatwave_happened = false;


        sf::RectangleShape flash;
        int flash_opacity = 255;

        float time = 0.f;
        sf::Clock clock;
};