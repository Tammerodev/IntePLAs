#pragma once
#include "Sand.hpp"
#include "math.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class Snow : public Sand {
    public:
        Snow(int x, int y) : Sand(x, y) {
            this->x = x;
            this->y = y;  

            terminal_velocity = 1;  

            color = elm::getInfoFromType(VoxelValues::SNOW).color;
            value = VoxelValues::SNOW;
            temperature = -5;
        }

        void sandUpdate(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            Voxel &vox = world.getVoxelAt(nextWaterPos.x, nextWaterPos.x); 
            if(vox.temp > 0) {
                remove = true;
                vox.temp = 30;
                vox.value = VoxelValues::WATER;
                world.setImagePixelAt(x, y, elm::getInfoFromType(VoxelValues::WATER).color);

            }
        }

        std::shared_ptr<Element> turn_into() {
            if(remove) {
                if(turn_to_glass) 
                    return std::make_shared<Water>(x, y);
            }
            return nullptr;
        }
};