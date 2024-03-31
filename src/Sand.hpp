#pragma once
#include "GravitySand.hpp"
#include "math.hpp"
#include "MoltenGlass.hpp"

class Sand : public GravitySand {
    public:
        Sand(int x, int y) : GravitySand(x, y) {
            this->x = x;
            this->y = y;    

            color = elm::getInfoFromType(VoxelValues::SAND).color;
            value = VoxelValues::SAND;
        }

        virtual std::shared_ptr<Element> turn_into() {
            if(remove) {
                if(turn_to_glass) 
                    return std::make_shared<MoltenGlass>(x + 1, y);
            }
            return nullptr;
        }

        void custom_update(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {
            if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).temp > melting_point) {
                turn_to_glass = true;
                remove = true;
                world.boundSetImagePixelAt(nextWaterPos.x, nextWaterPos.y, sf::Color(0,0,0,0));
            }

            sandUpdate(world, nextWaterPos);
        }

        virtual void sandUpdate(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {}; 

        bool clear() {
            return remove;
        }
        
    protected:
        bool turn_to_glass = false;
        bool remove = false;

        const int melting_point = 200;
};