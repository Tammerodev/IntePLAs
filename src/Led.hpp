#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"

class Led : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            voltage = world.boundGetVoxelAt(x, y).voltage;

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
            return false;
        }
    private:
        int treshold = 5;
        int voltage = 0;
};