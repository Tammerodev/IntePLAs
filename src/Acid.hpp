#pragma once
#include "Fluid.hpp"

class Acid : public Element {
    public:
        Acid(int xpos, int ypos) {
            x = xpos;
            y = ypos;
        }

        void update(ChunkIndexer& world) {
            sf::Vector2i lastPos = *this;

            world.boundSetImagePixelAt(x, y, elm::Acid);
            world.boundGetVoxelAt(x, y).value = elm::ValAcid;

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

            world.boundGetVoxelAt(x + 1, y).voltage = generated_voltage;
            world.boundGetVoxelAt(x - 1, y).voltage = generated_voltage;
            world.boundGetVoxelAt(x, y + 1).voltage = generated_voltage;
            world.boundGetVoxelAt(x, y - 1).voltage = generated_voltage;
            

            if(corrosion > 0) {
                sf::Vector2i bounded = *this;

                if(world.boundGetVoxelAt(x, y + 1).value != elm::ValAcid && world.boundGetVoxelAt(x, y + 1).value != elm::ValCarbon)
                    world.boundDamageVoxelAt(x, y + 1);

                if(world.boundGetVoxelAt(x, y + 1).value == 0) {
                    corrosion--;
                }
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

            world.boundGetVoxelAt(x, y).value = elm::ValAcid;

            world.boundSetImagePixelAt(x, y, elm::Acid);
        }

private:
    int corrosion = 10;
    int generated_voltage = 50;
};