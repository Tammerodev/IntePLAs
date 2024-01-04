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
                elm::Carbon,
                elm::Sand,
                elm::Sand
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Carbon,
                elm::Sand,
                elm::Sand
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Carbon,
                elm::Titanium,
                elm::Aluminium
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Carbon,
                elm::Titanium,
                elm::Aluminium
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Carbon,
                elm::Titanium,
                elm::Aluminium
            }
        };
    }
};