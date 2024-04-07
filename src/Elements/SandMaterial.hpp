#pragma once
#include "Element.hpp"
#include "../math.hpp"
#include "../ChunkIndexerVoxelContainer.hpp"
#include "GravityElement.hpp"

class SandMaterial : public GravityElement {
    public:
        SandMaterial(int x, int y) : GravityElement(x, y) {
            this->x = x;
            this->y = y;    
        }

        void run_rules(ChunkIndexer& world, sf::Vector2i& next_pos) {
            

            if(world.boundGetVoxelAt(next_pos.x + 1, next_pos.y + 1).value == 0) {
                next_pos.x = next_pos.x + 1;
                next_pos.y = next_pos.y + 1;
            } else if(world.boundGetVoxelAt(next_pos.x - 1, next_pos.x + 1).value == 0) {
                next_pos.x = next_pos.x - 1;
                next_pos.y = next_pos.y + 1;
            }

        }

        void custom_update(ChunkIndexer& world, sf::Vector2i& next_pos) {
            fluidUpdate(world);
        }

        virtual void fluidUpdate(ChunkIndexer& world) = 0;

        void move_(sf::Vector2i& nextWaterPos);
        
    protected:
};