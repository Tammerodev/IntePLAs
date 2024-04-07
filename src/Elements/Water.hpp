#pragma once
#include "GravityFluid.hpp"

class Water : public GravityFluid {
    public:
        Water(int xpos, int ypos) : GravityFluid(xpos, ypos) {
            color = elm::getInfoFromType(VoxelValues::WATER).color;
            value = VoxelValues::WATER;

            temperature = 5;
        }

        void fluidUpdate(ChunkIndexer& world) {
            
        }

    private:
        bool turn_to_glass = false;
        bool remove = false;

        const int melting_point = 2000;
};