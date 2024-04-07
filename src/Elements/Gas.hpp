#pragma once
#include "Element.hpp" 
#include "../math.hpp"
#include "GravityElement.hpp"

class Gas : public GravityElement {
    public:
        Gas(int x, int y) : GravityElement(x, y) {
            this->x = x;
            this->y = y;    

            velocity = {0,0};

            terminal_velocity = 2;
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

        void move_(sf::Vector2i& nextWaterPos) {
            velocity.y += 1;
            nextWaterPos -= velocity;
        }

        void custom_update(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            gasUpdate(world);
        }
        
        virtual void gasUpdate(ChunkIndexer& world) {
            
        }
        
    protected:
};