#pragma once
#include "GravityFluid.hpp"

class MoltenMetal : public GravityFluid {
    public:
        MoltenMetal(int xpos, int ypos, int type, int temp) : GravityFluid(xpos, ypos) {
            color = elm::getInfoFromType(type).color;
            value = type;
            viscosity = 10;

            temperature = temp;
        }

        void fluidUpdate(ChunkIndexer& world) {
            temperature--;
            viscosity++;

            if(temperature < 100 && world.boundGetVoxelAt(x, y + 1).value != 0) {
                if(world.boundGetVoxelAt(x, y + 1).value == 6 && timeOnAlum < 10) { // Al
                    timeOnAlum++;
                } else 
                    remove = true;
            }
        }

    private:
        int timeOnAlum = 0;
        const int melting_point = 2000;
};