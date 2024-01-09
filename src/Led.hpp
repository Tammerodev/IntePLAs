#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"

class Led : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            rem = voxel.value == 0;
    
            voltage = std::max({
                voxel.value,
                world.boundGetVoxelAt(x + 1, y).value,
                world.boundGetVoxelAt(x - 1, y).value,
                world.boundGetVoxelAt(x, y + 1).value,
                world.boundGetVoxelAt(x, y - 1).value
            });



            sf::Color color = sf::Color(14, 15, 23);

            if(voltage > treshold) {
                color = sf::Color::Red;
            }

            world.boundSetImagePixelAt(x, y, color);

            world.boundGetVoxelAt(x, y).voltage = 0;

            component_data data {};
            return data;
        }   

        int getVoltage() {
            return voltage;
        }

        void setVoltage(int) {

        }

        bool clear() {
            return rem;
        }
    private:
        bool rem = false;

        int treshold = 5;
        int voltage = 0;
};