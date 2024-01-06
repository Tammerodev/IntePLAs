#pragma once
#include "Fluid.hpp"
#include <algorithm>

class MoltenGlass : public Fluid {
    public:
        MoltenGlass(int xpos, int ypos) : Fluid(xpos, ypos){
            color = sf::Color(10, 10, 10, 20);
            value = elm::ValGlass;
        }

        void fluidUpdate(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            color = sf::Color(std::clamp((int)voxel.temp, 0, 255), 10, 10, 20);

            voxel.temp -= 5;

            if(voxel.temp < 1000)
                solid = true;
        }

        bool clear() {
            return solid;
        }

    private:
        bool solid = false;
};