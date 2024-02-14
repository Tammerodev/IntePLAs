#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"
#include "common.hpp"

class Oscillator : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            int time = timer.getElapsedTime().asMilliseconds();

            if(time > start_time) {
                voxel.value = VoxelValues::COPPER;
                world.boundSetImagePixelAt(x, y, sf::Color(68, 14, 71));
                std::cout << ("START") << std::endl;                    

                if(time > end_time) {
                    voxel.value = VoxelValues::OSCILLATOR;
                    world.boundSetImagePixelAt(x, y, elm::getInfoFromType(VoxelValues::OSCILLATOR).color);

                    timer.restart();

                    std::cout<<("END")<< std::endl;                    
                }
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
        sf::Clock timer;
        int start_time = 500;
        int end_time = 1000;

};