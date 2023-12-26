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
            std::array<sf::Color, 6> {
                elm::Sand,
                elm::Sand,
                elm::Sand,          
                elm::Sand,        
                elm::Sand,
                elm::Sand
            },
            
            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Sand,
                elm::Sand
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Sand,
                elm::Sand
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Titanium,
                elm::Aluminium
            },

            std::array<sf::Color, 6> {
                elm::Lead,
                elm::Carbon,
                elm::Lead,
                elm::Titanium,
                elm::Aluminium
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Titanium,
                elm::Aluminium
            }
        };
    }
};