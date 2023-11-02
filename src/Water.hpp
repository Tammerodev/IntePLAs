#pragma once
#include "Fluid.hpp"

class Water : public Fluid {
    public:
        Water(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::Water;
            color.a += math::randIntInRange(0, 20);
            value = elm::ValWater;
        }
};