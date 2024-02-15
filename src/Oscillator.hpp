#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"
#include "common.hpp"

class Oscillator : public ElectricComponent {
    public:
        Oscillator() {
            timer.restart();
        }

        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            int time = timer.getElapsedTime().asMilliseconds();
            int sine_value = std::sin(time * ((float)frequency / 1000.0f)) * 100;

            if(sine_value > treshold) {
                voxel.value = VoxelValues::COPPER;
                world.boundSetImagePixelAt(x, y, sf::Color(68, 14, 71));    
            } else if(sine_value < -treshold) {
                voxel.value = VoxelValues::OSCILLATOR;
                world.boundSetImagePixelAt(x, y, elm::getInfoFromType(VoxelValues::OSCILLATOR).color);                  
            } 

            component_data data {};
            return data;
        }   

        int getVoltage() {
            return 0;
        }

        void setVoltage(int) {

        }

        bool clear() {
            return rem;
        }
    private:
        bool rem = false;

        int treshold = 5;
        int frequency = 3;
        sf::Clock timer;
};