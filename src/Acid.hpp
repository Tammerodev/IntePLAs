#pragma once
#include "Fluid.hpp"

class Acid : public Fluid {
    public:
        Acid(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::Acid;
            value = elm::ValAcid;
            
            corrosion = 30;
        }
};