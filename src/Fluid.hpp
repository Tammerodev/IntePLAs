#pragma once
#include "Element.hpp"
#include "math.hpp"

class Fluid : public Element {
    public:
        Fluid(int x, int y) : Element() {
            this->x = x;
            this->y = y;    
        }

        virtual void fluidUpdate(ChunkIndexer& world) {}

        void update(ChunkIndexer& world) {

            if(world.boundGetVoxelAt(x, y).value == 0) {
                remove = true;
                return;
            }
            
            
            sf::Vector2i lastPos = *this;

            world.boundSetImagePixelAt(x, y, color);
            world.boundGetVoxelAt(x, y).value = VoxelValues::WATER;

            sf::Vector2i nextWaterPos = *this;

            nextWaterPos.y++;

            fluidUpdate(world);

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
            //temp = world.boundGetVoxelAt(x, y).temp;

            if(doesCooldown) {
                const short t = 1;
            
                world.SetHeat(x + 1, y, t);
                world.SetHeat(x - 1, y, t);

                world.SetHeat(x, y + 1, t);
                world.SetHeat(x, y - 1, t);

                world.SetHeat(x + 1, y + 1, t);
                world.SetHeat(x + 1, y - 1, t);

                world.SetHeat(x - 1, y + 1, t);
                world.SetHeat(x - 1, y - 1, t);
            }
            

            if(corrosion != 0) {
                sf::Vector2i bounded = *this;

                if(world.boundGetVoxelAt(x, y + 1).value != VoxelValues::WATER)
                    world.boundDamageVoxelAt(x, y + 1);
                
            }

            if(nextWaterPos != lastPos) {
                sf::Vector2i boundPos = lastPos;
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
            else 
                world.getChunkAt(world.getChunkFromPos(lastPos.x, lastPos.y)).needs_update = false;

            world.boundGetVoxelAt(x, y).value = value;

            world.boundSetImagePixelAt(x, y, color);
        }

        bool clear() {
            return remove;
        }
        
    protected:
        bool doesCooldown = true;

        int corrosion = 0;

        bool remove = false;

        short temp = 0;
        short gasTemp = 100;

        sf::Color color;
        uint8_t value;

};