#pragma once
#include "GameEvent.hpp"
#include "common.hpp"
#include <iostream>
#include "EventGlobals.hpp"

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

            pos = EventGlobals::position;
            started = true;

            SFX::nuclear_explosion.play();
        }

        void update(VoxelManager& vx) {

            if(started == false) {
                clock.restart();
                return;
            }            
            time = clock.getElapsedTime().asMilliseconds();


            if(time > 50 && heatwave_happened == false) {
                for(int i = 0; i < 6; i++) {
                    sf::Vector2i r_pos = pos + sf::Vector2i(math::randIntInRange(0, 1500), math::randIntInRange(0, 1500));
                    vx.holeRayCast(r_pos, 500, true, 0, 0, true);
                }
                heatwave_happened = true;
            }

            if(time > 50) {
                flash_opacity -= 2;
                if(flash_opacity < 0) 
                    flash_opacity = 0;
            }

            if(time > 10 && explosion_happened == false) {
                vx.holeRayCast(pos, 2000, true, 10000, 0, true);
                for(int i = 0; i < 200; i++) {
                    sf::Vector2i r_pos = pos + sf::Vector2i(math::randIntInRange(0, 2000), math::randIntInRange(0, 2000));
                    vx.getChunkIndexer().boundVector(r_pos);
                }
                explosion_happened = true;
            }

            flash.setFillColor(sf::Color(flash.getFillColor().r, flash.getFillColor().g, flash.getFillColor().b, flash_opacity));
        }

        void render(sf::RenderTarget& target) {
            target.draw(flash);
        }

    private:
        bool started = false;
        sf::Vector2i pos {5000, 500};

        bool explosion_happened = false;
        bool heatwave_happened = false;


        sf::RectangleShape flash;
        int flash_opacity = 255;

        float time = 0.f;
        sf::Clock clock;
};