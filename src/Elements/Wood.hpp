#pragma once
#include "Flammable.hpp"

class Wood : public Flammable {
    public:
        Wood(int x, int y) : Flammable(x, y) {
            value = VoxelValues::WOOD;
            ignition_temp = 350;
        }
    private:
};