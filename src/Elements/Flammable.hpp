#pragma once
#include "../common.hpp"

#include "BurnedMaterial.hpp"
#include "../FireEffectManager.hpp"

class Flammable : public Element {
    public:
        Flammable(int xpos, int ypos) {
            x = xpos;
            y = ypos;
        }
        

        int energy = 10;

        void update(ChunkIndexer& world) {
            world_local = &world;

            temp = world.boundGetVoxelAt(x, y).temp;

            if(temp > ignition_temp) {

                sf::Vector2i boundPos = *this;
                world.boundVector(boundPos);

                sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);
                world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;

                world.boundHeatVoxelAt(x,     y, energy);
                world.boundHeatVoxelAt(x + 1, y, energy);
                world.boundHeatVoxelAt(x - 1, y, energy);
                world.boundHeatVoxelAt(x, y + 1, energy);
                world.boundHeatVoxelAt(x, y - 1, energy);
                
                FireGlobal::add_source(*this, burning_energy);

                if(energy < 250) {
                    energy += 1;
                } else {
                    temp += 100;
                }

                if(temp > breakdown_temp || world.getVoxelAt(boundPos.x, boundPos.y).value == 0) {
                    remove = true;
                    world.clearVoxelAt(boundPos.x, boundPos.y);
                }
            }
        }   

        std::shared_ptr<Element> turn_into() {
            /*if(remove ) {
                if(world_local != nullptr) 
                    return std::make_shared<BurnedMaterial>(x + 1, y);
            }*/
            return nullptr;
        }

        bool clear() {
            return remove;
        }

        virtual void customUpdate(ChunkIndexer& world) {

        }

protected:
    bool remove = false;

    int ignition_temp = 300;
    int breakdown_temp = 10000;
    int burning_energy = 5;

    int temp = 0;

    ChunkIndexer* world_local;
};