#pragma once
#include "../common.hpp"

#include "BurnedMaterial.hpp"
#include "../FireEffectManager.hpp"
#include "GravityFluid.hpp"
#include "../FireEffectManager.hpp"

class FlammableFluid : public GravityFluid {
    public:
        FlammableFluid(int xpos, int ypos) : GravityFluid(xpos, ypos) {
            x = xpos;
            y = ypos;
        }

        void fluidUpdate(ChunkIndexer& world) {
            world_local = &world;

            short &temp = world.boundGetVoxelAt(x, y).temp;

            if(temp > ignition_temp) {

                sf::Vector2i boundPos = *this;
                world.boundVector(boundPos);
                sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);
                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;
                

                const int energy = ignition_temp + 50;
                
                world.boundHeatVoxelAtAndAdd(x, y, energy);
                world.boundHeatVoxelAtAndAdd(x + 1, y, energy);
                world.boundHeatVoxelAtAndAdd(x - 1, y, energy);
                world.boundHeatVoxelAtAndAdd(x, y + 1, energy);
                world.boundHeatVoxelAtAndAdd(x, y - 1, energy);

                FireGlobal::add_source(*this, burning_energy);

                world.clearVoxelAt(x, y);
                remove = true;
            }

            world.boundVector(*this);

            const sf::Vector2i& boundPos = *this;
            const sf::Vector2i& chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);

            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    world.boundGetChunkAt(chunk_pos.x + dx, chunk_pos.y + dy).needs_update = true;
                }
            }
        }   


protected:
    int ignition_temp = 300;
    int burning_energy = 100;

    ChunkIndexer* world_local;
};