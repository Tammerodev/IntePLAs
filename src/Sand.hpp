#pragma once

#pragma once
#include "Element.hpp"
#include "math.hpp"

class Sand : public Element {
    public:
        Sand(int x, int y) {
            this->x = x;
            this->y = y;    
        }

        void update(ChunkIndexer& world) {
            
            sf::Vector2i lastPos = {0,0};

            world.boundSetImagePixelAt(x, y, color);
            world.boundGetVoxelAt(x, y).value = elm::ValWater;

            sf::Vector2i nextWaterPos = *this;

            nextWaterPos.y++;


            if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
                world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
                
                world.boundGetVoxelAt(x, y).value = 0;

                x = nextWaterPos.x;
                y = nextWaterPos.y;

            } else {
                nextWaterPos.y--;
                int res = 0;

                if(world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                    res = 1;
                }
                if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0) {
                    res = -1;
                }
                if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0 && world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                    res = math::randIntInRange(-1, 1);
                }

                nextWaterPos.x += res;

                if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
                    world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
                    world.getVoxelAt(x, y).value = 0;
                        
                    x = nextWaterPos.x;
                    y = nextWaterPos.y;
                }
            }
            temp = world.boundGetVoxelAt(x, y).temp;

            // Spaghetti nam nam

            const short t = 1;
        
            world.SetHeat(x + 1, y, t);
            world.SetHeat(x - 1, y, t);

            world.SetHeat(x, y + 1, t);
            world.SetHeat(x, y - 1, t);

            world.SetHeat(x + 1, y + 1, t);
            world.SetHeat(x + 1, y - 1, t);

            world.SetHeat(x - 1, y + 1, t);
            world.SetHeat(x - 1, y - 1, t);

            world.boundGetVoxelAt(x, y).value = value;

            world.boundSetImagePixelAt(x, y, color);

            lastPos = *this;
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