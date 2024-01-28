#pragma once
#include "Gas.hpp"

class Steam : public Gas {
    public:
        Steam(int x, int y) : Gas(x, y) {
            value = VoxelValues::STEAM;
            color = elm::getInfoFromType(VoxelValues::STEAM).color;
        }
    private:
};