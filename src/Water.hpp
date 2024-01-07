#pragma once
#include "Fluid.hpp"

class Water : public Fluid {
    public:
        Water(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::getInfoFromType(VoxelValues::WATER).color;
            value = VoxelValues::WATER;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};