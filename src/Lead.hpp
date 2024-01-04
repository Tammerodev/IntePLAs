#pragma once
#include "Fluid.hpp"
#include "ElectricityGlobal.hpp"
#include "common.hpp"


class Lead : public Element {
    public:
        Lead(int xpos, int ypos) {
            x = xpos;
            y = ypos;
        }

        void update(ChunkIndexer& world) {
            if(world.isInContactWithVoxel(*this, elm::ValAcid)) {
                // Lead - Acid battery
                ElectricityGlobal::add_source(sf::Vector2i(x,y), 50);
            }
        }

private:
    int corrosion = 10;
    int generated_voltage = 50;
};