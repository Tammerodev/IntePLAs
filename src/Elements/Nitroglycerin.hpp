#pragma once
#include "GravityFluid.hpp"

class Nitroglycerin : public GravityFluid {
    public:
        Nitroglycerin(int xpos, int ypos) : GravityFluid(xpos, ypos){
            color = elm::getInfoFromType(NITROGLYCERIN).color;
            value = VoxelValues::NITROGLYCERIN;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};