#pragma once
#include "GravityFluid.hpp"
#include "ElectricityGlobal.hpp"

class Acid : public GravityFluid {
    public:
        Acid(int xpos, int ypos) : GravityFluid(xpos, ypos) {
            x = xpos;
            y = ypos;

            color = elm::getInfoFromType(VoxelValues::ACID).color;
            value = VoxelValues::ACID;
        }


        void fluidUpdate(ChunkIndexer& world) {
            if(world.boundGetVoxelAt(x, y + 1).value != 0 && world.boundGetVoxelAt(x, y + 1).value != VoxelValues::ACID) {
                if(corrosion <= 0 ) return;
                world.boundDamageVoxelAt(x, y + 1);
                corrosion--;
            }

            if(world.isInContactWithVoxel(*this, VoxelValues::LEAD)) {
                ElectricityGlobal::add_source(*this, generated_voltage);
            }
        }

private:
    int corrosion = 10;
    int generated_voltage = 50;
};