#pragma once
#include "Fluid.hpp"

class Blood : public Fluid {
    public:
        Blood(int xpos, int ypos) : Fluid(xpos, ypos){
            color = elm::getInfoFromType(VoxelValues::BLOOD).color;
            prndd("ADADASDASDAS");
            value = VoxelValues::BLOOD;

            velocity = sf::Vector2i(1, -5);
        }

        void fluidUpdate(ChunkIndexer& world) {
            *this += velocity;
            //velocity.y += 1;

            prndd("ADADASDASDAS");

            if(world.boundGetVoxelAt(x, y + 1).value != 0 && velocity.y > 0 && world.boundGetVoxelAt(x, y + 1).value != VoxelValues::BLOOD) velocity.y = 0;
            if(world.boundGetVoxelAt(x + 1, y).value != 0) velocity.x = 0;
            if(world.boundGetVoxelAt(x - 1, y).value != 0) velocity.x = 0;

        }

    private:
        sf::Vector2i velocity;
};