#pragma once
#include "Fluid.hpp"
#include "ElectricityGlobal.hpp"
#include "common.hpp"

#include "BurnedMaterial.hpp"
#include "FireEffectManager.hpp"
#include "Gas.hpp"

class FlammableGas : public Gas {
    public:
        FlammableGas(int xpos, int ypos) : Gas(x,y) {
            x = xpos;
            y = ypos;
        }

        void gasUpdate(ChunkIndexer& world) {
            world_local = &world;

            temp = world.boundGetVoxelAt(x, y).temp;

            if(temp > ignition_temp) {

                sf::Vector2i boundPos = *this;
                world.boundVector(boundPos);
                sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);
                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;
                

                const int energy = ignition_temp;

                world.boundHeatVoxelAt(x + 1, y, energy);
                world.boundHeatVoxelAt(x - 1, y, energy);
                world.boundHeatVoxelAt(x, y + 1, energy);
                world.boundHeatVoxelAt(x, y - 1, energy);

                //if(math::randIntInRange(1, 25) == 1) FireGlobal::add_source(sf::Vector2i(x, y));

                if(temp > breakdown_temp) {
                    _remove = true;
                    world.boundGetVoxelAt(x, y).temp = 0;
                    world.boundGetVoxelAt(x, y).value = VoxelValues::SAND;
                }
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

        std::shared_ptr<Element> turn_into() {
            return nullptr;
        }

        bool clear() {
            return _remove;
        }

protected:

    bool _remove = false;

    int ignition_temp = 300;
    int breakdown_temp = 10000;

    int temp = 0;

    ChunkIndexer* world_local;
};