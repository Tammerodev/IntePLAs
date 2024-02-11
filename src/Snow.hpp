#pragma once
#include "Sand.hpp"
#include "math.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class Snow : public Sand {
    public:
        Snow(int x, int y) : Sand(x, y) {
            this->x = x;
            this->y = y;    

            color = elm::getInfoFromType(VoxelValues::SNOW).color;
            value = VoxelValues::SAND;
        }
};