#pragma once
#include "Biome.hpp"

class Ocean : public Biome {
public:

    const std::string getName() {
        return "Ocean";
    }


    const VegetationInfo getVegetationInfo() {
        VegetationInfo vegetationInfo;
        vegetationInfo.filepath = "res/img/Procedural/forest.png";
        vegetationInfo.amount = 5;

        return vegetationInfo;
    }

    Ocean() {
        
        frequency = 0.1f;
        amplitude = 100.f;

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