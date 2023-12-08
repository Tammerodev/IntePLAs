#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"

class Led : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            world.boundGetVoxelAt(x, y).voltage = math::randIntInRange(0, 10);
            sf::Color color = sf::Color(14, 15, 23);

            if(world.boundGetVoxelAt(x, y).voltage > treshold) {
                color = sf::Color::Red;
            }
            

            world.boundSetImagePixelAt(x, y, color);
        }   

        int getVoltage() {

        }

        void setVoltage(int) {

        }

        bool clear() {

        }
    private:
        int treshold = 5;
};