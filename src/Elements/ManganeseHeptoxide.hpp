#pragma once
#include "FlammableFluid.hpp"

class ManganeseHeptoxide : public FlammableFluid {
    public:
        ManganeseHeptoxide(int x, int y) : FlammableFluid(x, y) {
            value = VoxelValues::MANGANESEHEPTOXIDE;
            color = elm::getInfoFromType(value).color;
            ignition_temp = 25;
        }
    private:
};