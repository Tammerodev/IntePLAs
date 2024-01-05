#pragma once
#include "Element.hpp"
#include "math.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class BurnedMaterial : public Element {
    public:
        BurnedMaterial(int x, int y) : Element() {
            this->x = x;
            this->y = y;    

            color = sf::Color(math::randIntInRange(0, 50), math::randIntInRange(0, 50), math::randIntInRange(0, 50), math::randIntInRange(50, 200));
            value = elm::ValSand;
        }

        void update(ChunkIndexer& world) {
            
            if(first_time) {
                world.boundGetVoxelAt(x, y).value = value;
                world.boundSetImagePixelAt(x, y, color);
            }
            first_time = false;


            if(world.boundGetVoxelAt(x, y).value == 0) {
                remove = true;
                return;
            }

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

                sf::Vector2i boundPos = *this;
                sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);

                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;
                world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y + 1).needs_update = true;
                world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y - 1).needs_update = true;

                world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y + 1).needs_update = true;
                world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y - 1).needs_update = true;

                world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y).needs_update = true;
                world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y).needs_update = true;

                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y + 1).needs_update = true;
                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y - 1).needs_update = true;
            }
            else {
                world.getChunkAt(world.getChunkFromPos(x, y)).needs_update = false;
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

        bool first_time = true;
};