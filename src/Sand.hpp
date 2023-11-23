#pragma once

#pragma once
#include "Element.hpp"
#include "math.hpp"

class Sand : public Element {
    public:
        Sand(int x, int y) {
            this->x = x;
            this->y = y;    

            color = elm::Sand;
            value = elm::ValSand;
        }

        void update(ChunkIndexer& world) {
            const sf::Vector2i previous_position = *this;


            // move
            y++;

            // Check if space isnt free, if not revert back
            // to previous position
            if(world.boundGetVoxelAt(x, y).value != 0) {
                x = previous_position.x;
                y = previous_position.y;

                if(world.boundGetVoxelAt(x + 1, y + 1).value == 0) {
                    x = x + 1;
                    y = y + 1;
                } else if(world.boundGetVoxelAt(x - 1, y + 1).value == 0) {
                    x = x - 1;
                    y = y + 1;
                }
            } 

            // Set pixel to sand in current position
            world.boundSetImagePixelAt(x, y, color);
            world.boundGetVoxelAt(x, y).value = value;

            if(*this != previous_position) {
                // We have moved, erase previous pixel
                world.boundSetImagePixelAt(previous_position.x, previous_position.y, sf::Color(0,0,0,0));
                world.boundGetVoxelAt(previous_position.x, previous_position.y).value = 0;
            }
        }

        bool clear() {
            return remove;
        }
        
    protected:
        bool remove = false;

        short temp = 0;
        sf::Color color;
        uint8_t value;
};