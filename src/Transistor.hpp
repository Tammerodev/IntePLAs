#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"

class TransistorDON : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            sf::Color color = elm::getInfoFromType(VoxelValues::TRANSISTORDON).color;

            if(voltage > treshold) {
                color.r = 100;
            }


            int base_voltage = world.boundGetVoxelAt(x - 1, y).voltage;
            int input_voltage = world.boundGetVoxelAt(x, y - 1).voltage;
            int output_voltage = world.boundGetVoxelAt(x, y + 1).voltage;

            if(base_voltage > treshold && input_voltage > treshold) {
                ElectricityGlobal::add_source(sf::Vector2i(x, y + 1), input_voltage);
            } else {
                ElectricityGlobal::remove_source(sf::Vector2i(x, y + 1));
            }

            world.boundSetImagePixelAt(x, y, color);

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


class TransistorDOFF : public ElectricComponent {
    public:
        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            sf::Color color = elm::getInfoFromType(VoxelValues::TRANSISTORDOFF).color;

            if(voltage < treshold) {
                color.r = 100;
            }


            int base_voltage = world.boundGetVoxelAt(x - 1, y).voltage;
            int input_voltage = world.boundGetVoxelAt(x, y - 1).voltage;
            int output_voltage = world.boundGetVoxelAt(x, y + 1).voltage;

            if(base_voltage > treshold || input_voltage < treshold) {
                ElectricityGlobal::remove_source(sf::Vector2i(x, y + 1));
            } else if(input_voltage >  treshold) {
                ElectricityGlobal::add_source(sf::Vector2i(x, y + 1), input_voltage);
            }

            world.boundSetImagePixelAt(x, y, color);

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