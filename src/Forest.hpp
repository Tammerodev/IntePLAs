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
        vegetationInfo.amount = 5;

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
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                sf::Color(12, 104, 45),
                sf::Color(22, 84, 35)
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