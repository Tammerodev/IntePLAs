#pragma once
#include "Fluid.hpp"

class Nitroglycerin : public Fluid {
    public:
        Nitroglycerin(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::getInfoFromType(NITROGLYCERIN).color;
            value = VoxelValues::NITROGLYCERIN;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};