#pragma once
#include "Biome.hpp"

class Forest : public Biome {
public:

    const std::string getName() {
        return "Forest";
    }


    const VegetationInfo getVegetationInfo() {
        VegetationInfo vegetationInfo;
        vegetationInfo.filepath = "res/img/Procedural/forest.png";
        vegetationInfo.amount = 25;
        vegetationInfo.width_of_part = 16;
        

        return vegetationInfo;
    }

    Forest() {
        
        frequency = 15.0f;
        amplitude = 400.f;

        colorLayers = {
            std::array<sf::Color, 6> {
                sf::Color(12, 84, 35),
                sf::Color(22, 74, 35),
                sf::Color(12, 94, 45),          
                sf::Color(22, 104, 35),        
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },
            
            std::array<sf::Color, 6> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },

            std::array<sf::Color, 6> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },

            std::array<sf::Color, 6> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::array<sf::Color, 6> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::array<sf::Color, 6> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };
    }
};