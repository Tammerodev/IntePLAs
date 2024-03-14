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

        std::shared_ptr<Element> turn_into() {
            if(remove ) {
                if(turn_to_glass) 
                    return std::make_shared<MoltenGlass>(x + 1, y);
            }
            return nullptr;
        }

        bool clear() {
            return remove;
        }
        
    protected:
        bool turn_to_glass = false;
        bool remove = false;

        const int melting_point = 5000;

        short temp = 0;
};