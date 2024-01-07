#pragma once
#include "Fluid.hpp"
#include "ElectricityGlobal.hpp"

class Acid : public Fluid {
    public:
        Acid(int xpos, int ypos) : Fluid(xpos, ypos) {
            x = xpos;
            y = ypos;

            color = elm::getInfoFromType(VoxelValues::ACID).color;
            value = VoxelValues::ACID;
        }


        void fluidUpdate(ChunkIndexer& world) {

            if(world.boundGetVoxelAt(x, y + 1).value != 0) {
                if(corrosion <= 0 ) return;
                world.boundDamageVoxelAt(x, y + 1);
                corrosion--;
            }
            
        }

private:
    int corrosion = 10;
    int generated_voltage = 50;
};