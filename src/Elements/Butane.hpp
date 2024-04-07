#pragma once
#include "FlammableGas.hpp"

class Butane : public FlammableGas {
    public:
        Butane(int x, int y) : FlammableGas(x, y) {
            color = elm::getInfoFromType(VoxelValues::BUTANE).color;
            value = VoxelValues::BUTANE;
        }
    private:
};