#pragma once
#include "GravityFluid.hpp"

class Water : public GravityFluid {
    public:
        Water(int xpos, int ypos) : GravityFluid(xpos, ypos) {
            color = elm::getInfoFromType(VoxelValues::WATER).color;
            value = VoxelValues::WATER;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};