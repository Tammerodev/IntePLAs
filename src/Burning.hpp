#pragma once
#include "Element.hpp"
#include "math.hpp"

class Burning : public Element {
    public:
        Burning(int x, int y) {
            this->x = x;
            this->y = y;   
        }

        void update(ChunkIndexer& world) {
            sf::Vector2i nextVoxelPos;
            int direction = math::randIntInRange(0, 3); 
            nextVoxelPos = *this;
            
            if(step) {
                if(direction == 0) nextVoxelPos.x += 1;
                if(direction == 1) nextVoxelPos.x -= 1;
                if(direction == 2) nextVoxelPos.y += 1;
                if(direction == 3) nextVoxelPos.y -= 1;

                step = false;
            }



            this->x = nextVoxelPos.x;
            this->y = nextVoxelPos.y;

            if(world.getVoxelAt(nextVoxelPos.x, nextVoxelPos.y).value == 0) {
                i--;
                if(i < 0) {
                    remove = true;
                }
                            
            } else {
                world.Heat(x, y, 50);
                if(world.getVoxelAt(nextVoxelPos.x, nextVoxelPos.y).value == 0) {
                    step = true;
                }
                i = 10;

            }
        }

        bool clear() {
            return remove;
        }
        
    protected:
        int i = 10;
        bool remove = false;

        bool step = false;

        sf::Color color;
        uint8_t value;
};