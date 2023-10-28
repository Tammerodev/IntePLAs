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

            int direction = math::randIntInRange(0, 3);
            
            if(step) {
                if(direction == 0) x += 1;
                if(direction == 1) x -= 1;
                if(direction == 2) y += 1;
                if(direction == 3) y -= 1;

                step = false;
            }

            if(world.getVoxelAt(x, y).value == 0) {
                remove = true;
            }

            world.Heat(x, y, 10);

            if(world.getVoxelAt(x, y).value == 0) {
                step = true;
            }
            
            
        }

        bool clear() {
            return remove;
        }
        
    protected:

        bool remove = false;

        bool step = false;

        sf::Color color;
        uint8_t value;
};