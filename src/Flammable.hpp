#pragma once
#include "Fluid.hpp"
#include "ElectricityGlobal.hpp"
#include "common.hpp"

#include "BurnedMaterial.hpp"
#include "FireEffectManager.hpp"

class Flammable : public Element {
    public:
        Flammable(int xpos, int ypos) {
            x = xpos;
            y = ypos;
        }

        void update(ChunkIndexer& world) {
            world_local = &world;

            temp = world.boundGetVoxelAt(x, y).temp;

            if(temp > ignition_temp) {

                sf::Vector2i boundPos = *this;
                world.boundVector(boundPos);
                sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);
                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;
                

                const int energy = 10;

                world.boundHeatVoxelAt(x + 1, y, energy);
                world.boundHeatVoxelAt(x - 1, y, energy);
                world.boundHeatVoxelAt(x, y + 1, energy);
                world.boundHeatVoxelAt(x, y - 1, energy);

                //if(math::randIntInRange(1, 25) == 1) FireGlobal::add_source(sf::Vector2i(x, y));

                if(temp > breakdown_temp) {
                    remove = true;
                    world.boundGetVoxelAt(x, y).temp = 0;
                    world.boundGetVoxelAt(x, y).value = VoxelValues::SAND;
                }
            }
        }   

        std::shared_ptr<Element> turn_into() {
            if(remove ) {
                if(world_local != nullptr && world_local->boundGetVoxelAt(x + 1, y).value != VoxelValues::SAND) 
                    return std::make_shared<BurnedMaterial>(x + 1, y);
            }
            return nullptr;
        }

        bool clear() {
            return remove;
        }

protected:

    bool remove = false;

    int value = VoxelValues::MAGNESIUM;
    int ignition_temp = 300;
    int breakdown_temp = 10000;

    int temp = 0;

    ChunkIndexer* world_local;
};