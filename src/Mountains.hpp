#pragma once
#include "Biome.hpp"

class Mountains : public Biome {
public:

    const std::string getName() {
        return "Mountains";
    }

    BiomeInfo info;

    const BiomeInfo* getInfo() {
        info.vegetationInfo.filepath = "res/img/Procedural/desert.png";
        info.vegetationInfo.amount = 1;
        info.vegetationInfo.offset_up = 2;
        info.vegetationInfo.height_of_part = 32;
        

        return &info;
    
    }

    Mountains() : Biome() {

        frequency = 50.0f;
        amplitude = 750.f;
        elevation = 1500;

        colorLayers = {
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),
                                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64),
                elm::getInfoFromType(VoxelValues::LEAD).color
            },
            
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,        
                sf::Color(66, 77, 69),
                sf::Color(62, 69, 64)
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