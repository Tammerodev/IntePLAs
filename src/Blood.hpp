#pragma once
#include "GravityFluid.hpp"
#include <SFML/Graphics/Vertex.hpp>

class Blood : public GravityFluid {
    public:
        Blood(int xpos, int ypos, const sf::Vector2i &vel) : GravityFluid(xpos, ypos){
            color = elm::getInfoFromType(VoxelValues::BLOOD).color;
            value = VoxelValues::BLOOD;

            velocity = vel;
        }

        void fluidUpdate(ChunkIndexer& world) {

        }
};