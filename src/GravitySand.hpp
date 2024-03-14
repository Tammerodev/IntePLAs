#pragma once
#include "GravityElement.hpp"

class GravitySand : public GravityElement {
    public:
        GravitySand(int xpos, int ypos) : GravityElement(xpos, ypos) {
            
        }

        void run_rules(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            if(world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y + 1).value == 0) {
                nextWaterPos.x = nextWaterPos.x + 1;
                nextWaterPos.y = nextWaterPos.y + 1;
            } else if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y + 1).value == 0) {
                nextWaterPos.x = nextWaterPos.x - 1;
                nextWaterPos.y = nextWaterPos.y + 1;
            } 
        }

        void custom_update(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            fluidUpdate(world);
        }

        virtual void fluidUpdate(ChunkIndexer& world) {

        }
    private:
};