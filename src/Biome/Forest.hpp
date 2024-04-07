#pragma once
#include "Biome.hpp"
#include "../MobType.hpp"

class Forest : public Biome {
public:

    const std::string getName() {
        return "Forest";
    }

    BiomeInfo info;

    const BiomeInfo* getInfo() {
        info.vegetationInfo.filepath = "res/img/Procedural/forest.png";
        info.vegetationInfo.amount = 25;
        info.vegetationInfo.width_of_part = 16;

        info.mobInfo.amount = 1;
        info.mobInfo.spawns.emplace_back(std::pair(MobType::Frog, 100));
        

        return &info;
    }

    Forest() {
        
        frequency = 15.0f;
        amplitude = 400.f;

        colorLayers = {
            std::vector<sf::Color> {
                sf::Color(12, 84, 35),
                sf::Color(22, 74, 35),
                sf::Color(12, 94, 45),          
                sf::Color(22, 104, 35),        
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },
            
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,

                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::MAGNESIUM).color

            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::URANIUM235).color,
                sf::Color(75,75,75),
                sf::Color(75,75,75),
                sf::Color(75,75,75)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color,
                elm::getInfoFromType(VoxelValues::MAGNESIUM).color
            }
        };
    }
};