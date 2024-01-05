#pragma once
#include "Fluid.hpp"

class Water : public Fluid {
    public:
        Water(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::Water;
            value = elm::ValWater;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};