#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"
#include "Lighting/LightGlobal.hpp"

class Led : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            rem = voxel.value == 0;
    
            voltage = std::max({
                world.boundGetVoxelAt(x + 1, y).value,
                world.boundGetVoxelAt(x - 1, y).value,
                world.boundGetVoxelAt(x, y + 1).value,
                world.boundGetVoxelAt(x, y - 1).value
            });


            sf::Color color = sf::Color(14, 15, 23);

            if(voltage > treshold) {
                color = sf::Color::Red;
                LightGlobal::add_source(*this, LightGlobal::LightElement(
                    50, sf::Color(255, 100, 100, 200)
                ));
            } else {
                LightGlobal::remove_source(*this);
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

        int treshold = 10;
        int voltage = 0;
};