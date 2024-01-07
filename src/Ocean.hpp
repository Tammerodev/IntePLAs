#pragma once
#include "Biome.hpp"

class Ocean : public Biome {
public:

    const std::string getName() {
        return "Ocean";
    }


    const VegetationInfo getVegetationInfo() {
        VegetationInfo vegetationInfo;
        vegetationInfo.filepath = "res/img/Procedural/ocean.png";
        vegetationInfo.amount = 25;
        vegetationInfo.offset_up = 2;

        return vegetationInfo;
    }

    Ocean() {
        
        frequency = 0.1f;
        amplitude = 100.f;
        elevation = 300.f;

        colorLayers = {
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color,          
                elm::getInfoFromType(VoxelValues::SAND).color,        
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color
            },
            
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };
    }
};