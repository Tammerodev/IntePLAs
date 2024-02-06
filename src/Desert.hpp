#pragma once
#include "Biome.hpp"

class Desert : public Biome {
public:

    const std::string getName() {
        return "Desert";
    }

    BiomeInfo info;

    const BiomeInfo* getInfo() {
        info.vegetationInfo.filepath = "res/img/Procedural/desert.png";
        info.vegetationInfo.amount = 1;
        info.vegetationInfo.offset_up = 2;
        info.vegetationInfo.height_of_part = 32;
        

        return &info;
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
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color>{
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };
    }
};