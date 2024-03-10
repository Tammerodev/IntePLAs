#pragma once
#include "GravityElement.hpp"

class GravityFluid : public GravityElement {
    public:
        GravityFluid(int xpos, int ypos) : GravityElement(xpos, ypos) {
            
        }

        void run_rules(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            int res = 0;

            if(world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                res = 1;
            }
            if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0) {
                res = -1;
            }
            if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0 && world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                res = math::randIntInRange(-1, 1);
            }

            nextWaterPos.x += res;
        }

        void custom_update(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            fluidUpdate(world);
        }

        virtual void fluidUpdate(ChunkIndexer& world) {

        }
    private:
};