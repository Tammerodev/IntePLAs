#pragma once
#include "Fluid.hpp"

class Blood : public Fluid {
    public:
        Blood(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::getInfoFromType(VoxelValues::BLOOD).color;
            value = VoxelValues::BLOOD;

            doesCooldown = false;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }

    private:
        sf::Vector2i velocity;
};