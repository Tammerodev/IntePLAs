#pragma once
#include "Biome.hpp"

class Desert : public Biome {
public:

    const std::string getName() {
        return "Desert";
    }

    const VegetationInfo getVegetationInfo() {
        VegetationInfo vegetationInfo;
        vegetationInfo.filepath = "res/img/Procedural/desert.png";
        vegetationInfo.amount = 1;
        vegetationInfo.offset_up = 2;
        vegetationInfo.height_of_part = 32;
        

        return vegetationInfo;
    }

    Desert() : Biome() {

        frequency = 10.0f;
        amplitude = 350.f;

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
                elm::getInfoFromType(VoxelValues::SAND).color,        
                elm::getInfoFromType(VoxelValues::SAND).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SAND).color,        
                elm::getInfoFromType(VoxelValues::SAND).color,
                elm::getInfoFromType(VoxelValues::SAND).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color>{
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::LEAD).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };
    }
};