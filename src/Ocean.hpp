#pragma once
#include "Biome.hpp"
#include "MobType.hpp"

class Ocean : public Biome {
public:

    const std::string getName() {
        return "Ocean";
    }
        
    BiomeInfo info;

    const BiomeInfo* getInfo() {
        info.vegetationInfo.filepath = "res/img/Procedural/ocean.png";
        info.vegetationInfo.amount = 25;
        info.vegetationInfo.offset_up = 2;

        info.mobInfo.amount = 1;
        info.mobInfo.spawns.emplace_back(std::pair(MobType::Fish, 100));

        return &info;
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
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };
    }
};