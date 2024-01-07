#pragma once
#include "Element.hpp"
#include "math.hpp"

class Gas : public Element {
    public:
        Gas(int x, int y) : Element() {
            this->x = x;
            this->y = y;    
        }

        void update(ChunkIndexer& world) {
            
            sf::Vector2i lastPos = {0,0};

            world.boundSetImagePixelAt(x, y, color);
            world.getVoxelAt(x, y).value = VoxelValues::WATER;

            sf::Vector2i nextWaterPos = *this;

            nextWaterPos.y--;
            if(world.getVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
                world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
                world.getVoxelAt(x, y).value = 0;

                x = nextWaterPos.x;
                y = nextWaterPos.y;

            } else {
                nextWaterPos.y++;
                int res = 0;

                if(world.getVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                    res = 1;
                }
                if(world.getVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0) {
                    res = -1;
                }
                if(world.getVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0 && world.getVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                    res = math::randIntInRange(-1, 1);
                }

                nextWaterPos.x += res;

                if(world.getVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
                    world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
                    world.getVoxelAt(x, y).value = 0;
                        
                    x = nextWaterPos.x;
                    y = nextWaterPos.y;

                }
            }

            world.getVoxelAt(x, y).value = value;
            world.boundSetImagePixelAt(x, y, color);

            lastPos = *this;
        }
        
    protected:
        sf::Color color;
        uint8_t value;
};